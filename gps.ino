#include <WiFi.h>
String Event_Name = "gps";
String Key = "***************************";//Yor Key
String resource = "/trigger/" + Event_Name + "/with/key/" + Key;
const char* server = "maker.ifttt.com";
const char* ssid     = "Username";
const char* password = "PASS";


void setup() {
  // put your setup code here, to run once:
    pinMode(13,INPUT);
  pinMode(12,INPUT);
    Serial.begin(115200);
    Serial.print("Connecting to: ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);

  int timeout = 10 * 4; // 10 seconds
  while (WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect, going back to sleep");
  }

  Serial.print("WiFi connected in: ");
  Serial.print(millis());
  Serial.print(", IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  
 makeIFTTTRequest();
 delay(10000);

}
void makeIFTTTRequest() {
  int x=analogRead(13);
  int y=34;
  Serial.print("x--");
  Serial.println(x);
  Serial.print("y--");
  Serial.println(y);
  delay(1500);
  
  Serial.print("Connecting to ");
  Serial.print(server);

  WiFiClient client;
  int retries = 5;
  while (!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if (!!!client.connected()) {
    Serial.println("Failed to connect...");
  }

  Serial.print("Request resource: ");
  Serial.println(resource);
  String jsonObject = String("{\"value1\":\"") + x + "\",\"value2\":\"" + y
                      + "\"}";

  client.println(String("POST ") + resource + " HTTP/1.1");
  client.println(String("Host: ") + server);
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject.length());
  client.println();
  client.println(jsonObject);

  int timeout = 5 * 10; // 5 seconds
  while (!!!client.available() && (timeout-- > 0)) {
    delay(100);
  }
  if (!!!client.available()) {
    Serial.println("No response...");
  }
  while (client.available()) {
    Serial.write(client.read());
  }

  Serial.println("\nclosing connection");
  client.stop();
}
