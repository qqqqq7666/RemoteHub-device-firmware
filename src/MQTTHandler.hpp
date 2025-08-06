#include <PubSubClient.h>
#include <WiFi.h>

WiFiClient espClient;
PubSubClient client(espClient);

class MQTTHandler {
 private:
  const char *mqttServer = "broker.hivemq.com";  // MQTT 브로커 주소
  const int mqttPort = 1883;                     // MQTT 포트
  const char *mqttUser = "";                     // MQTT 사용자 이름 (필요시 설정)
  const char *mqttPassword = "";                 // MQTT 비밀번호 (필요시 설정)

  void reconect() {
    Serial.print("Attempting MQTT connection...");
    if (!client.connected()) {
      if (client.connect("ESP32")) {
        Serial.println("connected");
      } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        delay(5000);
      }
    }
  }

 public:
  void connectMQTT() {
    while (!client.connected()) {
      reconect();
      Serial.print("Connecting to MQTT broker...");
      delay(1000);
    }
  }

  void subscribe() {
    client.subscribe("airconditioner/power", 1);
  }
};