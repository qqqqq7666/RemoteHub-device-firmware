#include "MQTTHandler.h"

#include <Arduino.h>

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

void MQTTHandler::callback(char* topic, byte* payload, unsigned int length) {
  String stringTopic = String(topic);
  String stringPayload;
  for (int i = 0; i < length; i++) {
    stringPayload.concat(payload[i]);
  }
  if(stringTopic.startsWith("userId/save")) {
    
  }
  if(stringTopic.startsWith("userId/control")) {

  }
  Serial.printf("\n[MQTT] Topic: %s | Length: %u\n", topic, length);
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}

String* splitMessage(String& message) {
  String res[3];
  int firstSpaceIdx = message.indexOf(" ");
  int secondSapceIdx = message.indexOf(" ", firstSpaceIdx + 1);

  res[0] = message.substring(0, firstSpaceIdx);
  res[1] = message.substring(firstSpaceIdx + 1, secondSapceIdx);
  res[2] = message.substring(secondSapceIdx + 1);
  return res;
}