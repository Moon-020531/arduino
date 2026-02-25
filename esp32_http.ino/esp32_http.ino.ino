#include <WiFi.h>

#include <HTTPClient.h>

const char* ssid = "5층";       // 수정 필요

const char* password = "48864886";  // 수정 필요

String serverURL = "http://192.168.0.80:5000/api/sensor";  // 수정 필요

void setup() {

  Serial.begin(115200);

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

  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    http.begin(serverURL);

    http.addHeader("Content-Type", "application/json");

    // 더미 데이터로 POST 전송

    String jsonData = "{\"temperature\":25.3,\"humidity\":60.5}";

    int httpCode = http.POST(jsonData);

    if (httpCode > 0) {

      String response = http.getString();

      Serial.print("서버 응답 코드: ");

      Serial.println(httpCode);

      Serial.print("서버 응답: ");

      Serial.println(response);

    } else {

      Serial.print("전송 실패, 에러코드: ");

      Serial.println(httpCode);

    }

    http.end();

  } else {

    Serial.println("WiFi 연결 끊김!");

  }

  delay(5000);

}
