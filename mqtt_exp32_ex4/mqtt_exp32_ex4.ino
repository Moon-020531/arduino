/******************************************************************************
 * step3_temperature_mqtt.ino
 * 3단계: 온도 센서 조건부 퍼블리시 (QoS 0)
 ******************************************************************************/

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// ===== Wi-Fi 설정 =====
const char *ssid     = "5층";       
const char *password = "48864886";    

// ===== MQTT 설정 =====
const char *MQTT_ID = "temperature_0205";          
const char *TOPIC   = "device/0205/temperature";   

IPAddress broker(192, 168, 0, 80);               
const int MQTT_PORT = 1883;

WiFiClient wclient;
PubSubClient client(wclient);

// ===== DHT 센서 설정 =====
#define DHT_PIN   4          // GPIO 4
#define DHT_TYPE  DHT11      // DHT11 또는 DHT22
DHT dht(DHT_PIN, DHT_TYPE);

// ===== 타이밍 설정 =====
unsigned long previousMillis = 0;
const long interval = 2000;   // 2초마다 측정

// ===== 조건부 전송 설정 =====
const float THRESHOLD_TEMP = 30.0;    // 이 온도 이상일 때만 전송

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

// ===== 전송 조건 판단 함수 =====
bool shouldPublish(float temperature) {
  // 방식 A: 임계값(30.0도) 이상일 때만 전송
  if (temperature >= THRESHOLD_TEMP) {
    return true;
  }
  return false; 
}

void setup() {
  Serial.begin(115200);

  dht.begin();

  delay(100);
  setup_wifi();
  client.setServer(broker, MQTT_PORT);

  Serial.println("3단계: 온도 조건부 MQTT 퍼블리시");
  Serial.println("=================================");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // 1. 온도 읽기
    float temperature = dht.readTemperature();

    // 2. 센서 읽기 실패(에러) 처리
    if(isnan(temperature)) {
      Serial.println("센서 읽기 X");
      return; 
    } 

    // 3. 온도를 문자열로 변환
    char tempStr[10];
    dtostrf(temperature, 4, 1, tempStr); 

    // 4. 전송 조건 확인
    if (shouldPublish(temperature)) {
      // 30도 이상이면 MQTT 전송
      client.publish(TOPIC, tempStr);

      Serial.print("[전송] ");
      Serial.print(TOPIC);
      Serial.print(" => ");
      Serial.print(tempStr);
      Serial.println(" C"); 
    } else {
      // 30도 미만이면 전송 스킵
      Serial.print("[스킵] 온도: ");
      Serial.print(tempStr); 
      Serial.println(" C - 조건 미충족");
    }
  }
}