#include <WiFi.h>  // ESP32용 라이브러리로 변경
#include <WiFiClient.h>

const char* ssid     = "5층";
const char* password = "48864886";

WiFiClient client;   
const char* host     = "192.168.0.2";
uint16_t port        = 9999;
char wbuf[20];

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();  
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WIFI connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  
  if (!client.connect(host, port)) {
    Serial.println("Connection failed");
    return;
  }
  Serial.println("Server connected!");
}

void loop() {
  int n = 0;
  if (Serial.available() > 0) {
    n = Serial.readBytes(wbuf, 20);
    wbuf[n] = '\0';
    
   
    if (client.connected()) {
      client.print(wbuf);
      Serial.print("Sent: ");
      Serial.println(wbuf);
    } else {
      Serial.println("Server disconnected");
    }
  }
  delay(1000); 
}