/******************************************************************************
 * step2_ultrasonic_mqtt.ino
 * 2단계: 초음파 데이터를 MQTT로 퍼블리시
 *
 * 목표: 초음파 거리 데이터를 MQTT 브로커(라즈베리파이)로 전송
 * 토픽: device/{student_id}/ultrasonic
 * QoS:  PubSubClient는 Publish QoS 0만 지원 (학습용으로 QoS 개념 이해)
 *
 * 배선: 1단계와 동일
 *   HC-SR04 VCC  → NodeMCU Vin (5V)
 *   HC-SR04 GND  → NodeMCU GND
 *   HC-SR04 TRIG → NodeMCU D1 (GPIO 5)
 *   HC-SR04 ECHO → NodeMCU D2 (GPIO 4) ※ 전압분배(1kΩ+2kΩ) 권장
 *
 * 확인: 라즈베리파이에서
 *   mosquitto_sub -t "device/01/ultrasonic" -v
 ******************************************************************************/

#include <WiFi.h>
#include <PubSubClient.h>

// ===== Wi-Fi 설정 (본인 환경에 맞게 수정) =====
const char *ssid     = "5층";       // TODO: Wi-Fi 이름
const char *password = "48864886";    // TODO: Wi-Fi 비밀번호

// ===== MQTT 설정 =====
const char *MQTT_ID = "ultrasonic_0205";          // TODO: 학생마다 고유하게 변경
const char *TOPIC   = "device/0205/ultrasonic";   // TODO: 01을 본인 번호로 변경

IPAddress broker(192, 168, 0, 80);              // TODO: 라즈베리파이 IP로 변경
const int MQTT_PORT = 1883;

WiFiClient wclient;
PubSubClient client(wclient);

// ===== 초음파 센서 핀 설정 =====
const int TRIG_PIN = 23;   // D1
const int ECHO_PIN = 22;   // D2

// ===== 타이밍 설정 =====
unsigned long previousMillis = 0;
const long interval = 2000;   // 2초마다 측정 및 전송


// ===== Wi-Fi 연결 =====
void setup_wifi() {
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// ===== MQTT 재연결 =====
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (client.connect(MQTT_ID)) {
      Serial.println("connected");
      Serial.print("Publishing to: ");
      Serial.println(TOPIC);
      Serial.println();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// ===== 초음파 거리 측정 함수 =====
// 1단계에서 완성한 함수를 그대로 가져온다
float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return -1.0;
  } else {
    return duration * 0.034 / 2.0;
  }
}

void setup() {
  Serial.begin(115200);

  // 초음파 센서 핀 설정
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  delay(100);
  setup_wifi();
  client.setServer(broker, MQTT_PORT);

  Serial.println("2단계: 초음파 MQTT 퍼블리시");
  Serial.println("============================");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float distance = measureDistance();

    if (distance > 0) {
      char distStr[10];
      dtostrf(distance, 4, 1, distStr);
      client.publish(TOPIC, distStr);

      Serial.print(TOPIC);
      Serial.print(" =>");
      Serial.print(distStr);
      Serial.println(" cm");
      //예: "device/01/ultrasonic => 23.5 cm"

    } else {
      Serial.println("측정 실패 (범위 초과)");
    }
  }
}