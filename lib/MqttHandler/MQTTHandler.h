#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <PubSubClient.h>
#include <WiFiClient.h>

class MQTTHandler {
 private:
  const char *mqttServer;
  const int mqttPort;

  WiFiClient espClient;
  PubSubClient client;

  void reconnect();
  static void callback(char* topic, byte* payload, unsigned int length);

 public:
  MQTTHandler(const char* server, int port);
  void connectMQTT();
  void subscribe(const char* topic, int qos = 0);
  void publish(const char* topic, const char* payload);
  PubSubClient& getClient(); // 외부에서 publish 등 사용 가능하도록
};

#endif
