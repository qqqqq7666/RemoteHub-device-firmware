#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <PubSubClient.h>
#include <WiFiClient.h>

class MQTTHandler {
 private:
  const char *mqttServer;
  const int mqttPort;
  const char *mqttUser;
  const char *mqttPassword;

  WiFiClient espClient;
  PubSubClient client;

  void reconnect();

 public:
  MQTTHandler(const char *server, int port, const char *user = "", const char *password = "");
  void connectMQTT();
  void subscribe(const char *topic, int qos = 0);
  PubSubClient& getClient(); // 외부에서 publish 등 사용 가능하도록
};

#endif
