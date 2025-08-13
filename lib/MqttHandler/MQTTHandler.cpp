#include "MQTTHandler.h"

#include <Arduino.h>

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.printf("\n[MQTT] Topic: %s | Length: %u\n", topic, length);
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}

MQTTHandler::MQTTHandler(const char *server, int port, const char *user, const char *password) : mqttServer(server), mqttPort(port), mqttUser(user), mqttPassword(password), client(espClient) {
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void MQTTHandler::reconnect() {
  Serial.print("Attempting MQTT connection...");
  if (!client.connected()) {
    if (client.connect("ESP32", mqttUser, mqttPassword)) {
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

PubSubClient &MQTTHandler::getClient() { return client; }