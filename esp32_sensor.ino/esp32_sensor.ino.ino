#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

const char* ssid = "5층";         // 수정 필요

const char* password = "48864886";  // 수정 필요

String serverURL ="http://192.168.0.80:5000/api/sensor"; // 수정 필요

#define DHTPIN 4

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(115200);

  dht.begin();

  WiFi.begin(ssid, password);

  Serial.print("WiFi 연결 중");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }

  Serial.println();

  Serial.print("WiFi 연결 완료! IP: ");

  Serial.println(WiFi.localIP());

}

void loop() {

  float humidity = dht.readHumidity();

  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {

    Serial.println("센서 읽기 실패!");

    delay(2000);

    return;

  }

  Serial.print("온도: ");

  Serial.print(temperature);

  Serial.print(" / 습도: ");

  Serial.println(humidity);

  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    http.begin(serverURL);

    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";

    int httpCode = http.POST(jsonData);

    if (httpCode > 0) {

      String response = http.getString();

      Serial.println("서버 응답: " + response);

    } else {

      Serial.println("전송 실패! 코드: " + String(httpCode));

    }

    http.end();

  }

  delay(5000);

}
