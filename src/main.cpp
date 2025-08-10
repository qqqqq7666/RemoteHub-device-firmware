#include <WifiHandler.hpp>
#include "config.h"
#include "IRHandler.h"
#include "MQTTHandler.h"
#include "WiFiProvisioning.h"

WifiHandler wifiHandler;
IRHandler irHandler(IR_RECV_PIN, IR_LED_PIN);
MQTTHandler mqttHandler(MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD);
WiFiProvisioning wiFiProvisioning("remote_hub");

void setup() {
  Serial.begin(115200);
  wifiHandler.setupWiFi();
  irHandler.setupIR();
  wiFiProvisioning.begin();
}

void loop() {
  server.handleClient();
  irHandler.handleIR();
  mqttHandler.connectMQTT();
  wiFiProvisioning.loop();
}