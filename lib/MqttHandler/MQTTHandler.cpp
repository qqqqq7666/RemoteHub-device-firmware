#include "MQTTHandler.h"
#include <Arduino.h>

MQTTHandler::MQTTHandler(const char *server, int port, const char *user, const char *password)
  : mqttServer(server),
    mqttPort(port),
    mqttUser(user),
    mqttPassword(password),
    client(espClient) {
  client.setServer(mqttServer, mqttPort);
}

void MQTTHandler::reconnect() {
  Serial.print("Attempting MQTT connection...");
  if (!client.connected()) {
    if (client.connect("ESP32", mqttUser, mqttPassword)) {
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
  while (!client.connected()) {
    reconnect();
    Serial.print("Connecting to MQTT broker...");
    delay(1000);
  }
}

void MQTTHandler::subscribe(const char *topic, int qos) {
  client.subscribe(topic, qos);
}

PubSubClient& MQTTHandler::getClient() {
  return client;
}