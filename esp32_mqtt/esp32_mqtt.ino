#include <WiFi.h>
#include <PubSubClient.h>

#define LED 2
bool ledState = false;

// WiFi settings
const char *ssid = "5층";             
const char *password = "48864886";   

// MQTT Broker settings
const char *mqtt_broker = "broker.emqx.io";  
const char *mqtt_topic = "emqx/esp";     
const char *mqtt_username = "emqx"; 
const char *mqtt_password = "public"; 
const int mqtt_port = 1883; 

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

void connectToWiFi();
void connectToMQTTBroker();
void mqttCallback(char *topic, byte *payload, unsigned int length);

void setup() {
    Serial.begin(115200);
    
    
    pinMode(LED, OUTPUT); 
    
    connectToWiFi();
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    mqtt_client.setCallback(mqttCallback);
    connectToMQTTBroker();
}

void connectToWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to the WiFi network");
}

void connectToMQTTBroker() {
    while (!mqtt_client.connected()) {
        String client_id = "esp32-client-" + String(WiFi.macAddress());
        Serial.printf("Connecting to MQTT Broker as %s.....\n", client_id.c_str());
        
        if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Connected to MQTT broker");
            mqtt_client.subscribe(mqtt_topic);
            mqtt_client.publish(mqtt_topic, "Hi EMQX I'm ESP32 ^^");
        } else {
            Serial.print("Failed to connect to MQTT broker, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message received on topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    
    String message;
    for (int i = 0; i < length; i++) {
        message += (char) payload[i];  
    }
    Serial.println(message); // 수신된 메시지를 시리얼 모니터에 출력
    
    if (message == "on" && !ledState) {
        digitalWrite(LED, HIGH);  
        ledState = true;
        Serial.println("LED is turned on");
    }
    if (message == "off" && ledState) {
        digitalWrite(LED, LOW); 
        ledState = false;
        Serial.println("LED is turned off");
    }
    
    Serial.println();
    Serial.println("-----------------------");
}

void loop() {
    if (!mqtt_client.connected()) {
        connectToMQTTBroker();
    }
    mqtt_client.loop();
}