#include "MQTTHandler.h"

#include <Arduino.h>

void MQTTHandler::callback(char *topic, byte *payload, unsigned int length) {
  Serial.printf("\n[MQTT] Topic: %s | Length: %u\n", topic, length);
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}


// TODO callback 함수 안불리는 문제 해결
MQTTHandler::MQTTHandler(const char *server, int port) : mqttServer(server), mqttPort(port), client(espClient) {
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void MQTTHandler::reconnect() {
  Serial.print("Attempting MQTT connection...");
  if (!client.connected()) {
    if (client.connect("ESP32")) {
      client.subscribe("device/#");
    } else {
      Serial.println("connected");
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void MQTTHandler::connectMQTT() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void MQTTHandler::subscribe(const char *topic, int qos) { client.subscribe(topic, qos); }
void MQTTHandler::publish(const char *topic, const char *payload) {
  client.publish(topic, payload);
}

PubSubClient &MQTTHandler::getClient() { return client; }