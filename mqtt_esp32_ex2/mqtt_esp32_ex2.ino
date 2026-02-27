/******************************************************************************
 * step1_ultrasonic.ino
 * 1단계: 초음파 센서 데이터 읽기 (로컬 테스트)
 *
 * 목표: HC-SR04 초음파 센서로 거리를 측정하고 시리얼 모니터에 출력
 * 이 단계에서는 MQTT를 사용하지 않는다.
 ******************************************************************************/

// ===== 초음파 센서 핀 설정 =====
const int TRIG_PIN = 23;   // D1 (ESP32 기준 GPIO 23)
const int ECHO_PIN = 22;   // D2 (ESP32 기준 GPIO 22)

// ===== 타이밍 설정 =====
unsigned long previousMillis = 0;
const long interval = 2000;   // 2초마다 측정

// ===== 초음파 거리 측정 함수 =====
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

  // 핀 모드 설정
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("1단계: 초음파 센서 테스트");
  Serial.println("========================");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float distance = measureDistance();

    if (distance > 0) {
      Serial.print("거리: ");
      Serial.print(distance, 1);   // 소수점 1자리까지 출력
      Serial.println(" cm");
    } else {
      Serial.println("측정 실패 (범위 초과)");
    }
  }
}