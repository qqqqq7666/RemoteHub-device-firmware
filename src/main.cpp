#include <WifiHandler.hpp>
#include <IRHandler.hpp>
#include <MQTTHandler.hpp>

WifiHandler wifiHandler;
IRHandler irHandler;
MQTTHandler mqttHandler;

void setup() {
  Serial.begin(115200);
  wifiHandler.setupWiFi();
  irHandler.setupIR();
}

void loop() {
  server.handleClient();
  irHandler.handleIR();
  mqttHandler.connectMQTT();
}