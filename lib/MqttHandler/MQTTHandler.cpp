#include "MQTTHandler.h"

#include <Arduino.h>

MQTTHandler::MQTTHandler(const char *server, int port, IRHandler& ir) 
: mqttServer(server), mqttPort(port), irHandler(ir), client(espClient) {
  client.setServer(mqttServer, mqttPort);
  client.setCallback([this](char *topic, byte *payload, unsigned int length) {
    this->handleMessage(topic, payload, length);
  });
}

void MQTTHandler::reconnect() {
  Serial.print("Attempting MQTT connection...");
  if (!client.connected()) {
    if (client.connect("ESP32")) {
      client.subscribe("remoteHubId123/save");
      client.subscribe("remoteHubId123/send");
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
void MQTTHandler::publish(const char *topic, const char *payload) { client.publish(topic, payload); }

PubSubClient &MQTTHandler::getClient() { return client; }

void MQTTHandler::handleMessage(char *topic, byte *payload, unsigned int length) {
  String stringTopic = String(topic);
  String stringPayload = String((char*)payload).substring(0, length);
  if (stringTopic.startsWith("remoteHubId123/save")) {
    // uint32_t signal = irHandler.getLastReceiveSignal();
    // http.begin(saveIrUrl);
    // http.addHeader("Content-Type", "application/json");
    // String body = "{\"signal\":\"" + signal + "\"}";
    // int code = http.POST(body);
    // Serial.println("Register response code: " + String(code));
    // http.end();
  }
  if (stringTopic.startsWith("remoteHubId123/send")) {
    uint32_t signal = strtoll(stringPayload.c_str(), NULL, 16);
    irHandler.send(signal);
  }
  Serial.printf("\n[MQTT] Topic: %s | Length: %u\n", topic, length);
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}

std::array<String, 3> splitMessage(const String& message) {
  std::array<String, 3> res;
  int firstSpaceIdx = message.indexOf(" ");
  int secondSapceIdx = message.indexOf(" ", firstSpaceIdx + 1);

  res[0] = message.substring(0, firstSpaceIdx);
  res[1] = message.substring(firstSpaceIdx + 1, secondSapceIdx);
  res[2] = message.substring(secondSapceIdx + 1);
  return res;
}