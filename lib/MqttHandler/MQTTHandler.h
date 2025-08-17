#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <PubSubClient.h>
#include <HTTPClient.h>
#include "IRHandler.h"

class MQTTHandler {
 private:
  const char* mqttServer;
  const int mqttPort;
  const char* mqttId;

  WiFiClient espClient;
  PubSubClient client;
  IRHandler& irHandler;
  HTTPClient http;

  void reconnect();
  static void callback(char* topic, byte* payload, unsigned int length);

 public:
  MQTTHandler(const char* server, int port, IRHandler& ir);
  void connectMQTT();
  void subscribe(const char* topic, int qos = 0);
  void publish(const char* topic, const char* payload);
  PubSubClient& getClient(); // 외부에서 publish 등 사용 가능하도록
  void handleMessage(char *topic, byte *payload, unsigned int length);
};

#endif
