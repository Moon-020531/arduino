#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

#define DHTTYPE DHT11
#define DHTPIN 4
const char* ssid = "5층";
const char* password = "48864886"; 

WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

float temp, humi;
String webString = "";
unsigned long previousMillis = 0; 
const long interval = 2000;

void gettemphumi();

void handleevents() {
  gettemphumi();
  webString = "{\"temperature\": \"" + String(temp) + "\", \"humidity\":\"" + String(humi) + "\"}"; 

  Serial.println(webString); 
  server.send(200, "application/json", webString); 
  yield();
}

void setup() {
  Serial.begin(115200); 
  dht.begin();          

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());

  server.on("/events", handleevents);
  server.begin();
}

void loop() {
  server.handleClient();
  // --- 이 부분이 추가되어야 웹 요청 없이도 실시간으로 센서를 읽습니다 ---
  gettemphumi(); 
}

void gettemphumi() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    humi = dht.readHumidity(); 
    temp = dht.readTemperature(false);

    if (isnan(humi) || isnan(temp)) {
      Serial.println("Failed to read dht sensor."); 
      return;
    }

    // 이제 loop()에서 호출되므로 2초마다 자동으로 찍힙니다.
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print("°C, Humidity: ");
    Serial.print(humi);
    Serial.println("%");
  }
}