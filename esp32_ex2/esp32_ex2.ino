#include <WiFi.h>
#include <WiFiMulti.h>
#include <SocketIoClient.h>
#include <DHT.h>

#define DHTTYPE DHT11
#define DHTPIN 4

WiFiMulti WiFiMulti;
SocketIoClient webSocket; 

const char* ssid = "5층";
const char* password = "48864886";


const char *serverIp ="192.168.0.80"; 
const int port = 5000;

DHT dht(DHTPIN, DHTTYPE);

float temp, humi;
String webString="";
char ch[100] ={0}; 
unsigned long previousMillis = 0; 
const long interval = 5000;
const int led = 2;


void handleconnect(const char * payload, size_t length){ 
  Serial.println("서버와 소켓 연결 성공!");
  

  webSocket.emit("join_dev", "{\"data\": \"nodeMCU\"}");
}


void handleled(const char * payload, size_t length) {
  Serial.printf("서버 명령 수신: %s\n", payload);
  
  if (strstr(payload, "ON")) {
    digitalWrite(led, HIGH);
    Serial.println("LED ON");
  } 

  else if (strstr(payload, "OFF")) {
    digitalWrite(led, LOW);
    Serial.println("LED OFF");
  }
}

void gettemphumi();

void handleevents(){
   gettemphumi(); 
   webString = "{\"temperature\": \"" + String(temp) + "\", \"humidity\":\"" + String(humi) + "\"}";
   
   webString.toCharArray(ch, 100); 
   
   Serial.print("데이터 전송: ");
   Serial.println(ch); 
   webSocket.emit("events", ch);
}

void setup() {
  pinMode(led, OUTPUT); 
  digitalWrite(led, LOW); 
  Serial.begin(115200);
  delay(10);
  dht.begin();

  Serial.print("WiFi 접속 중: "); 
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  while (WiFiMulti.run() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi 연결 완료!");
  Serial.print("IP 주소: ");
  Serial.println(WiFi.localIP());

  // 이벤트 핸들러 등록
  webSocket.on("connect", handleconnect);
  webSocket.on("led_control", handleled); 
  
  // 소켓 통신 시작 (서버 IP와 포트 확인)
  webSocket.begin(serverIp, port, "/socket.io/?EIO=3&transport=websocket");
}

void loop() {
  webSocket.loop(); 
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval){ 
    previousMillis = currentMillis;
    handleevents(); 
  }
}

void gettemphumi(){
  humi = dht.readHumidity();
  temp = dht.readTemperature(false);
  if(isnan(humi) || isnan(temp)){
    Serial.println("❌ 센서 읽기 실패!");
    return;
  }
}