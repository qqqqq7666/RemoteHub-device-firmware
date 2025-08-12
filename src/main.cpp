#include <Arduino.h>

#include "IRHandler.h"
#include "MQTTHandler.h"
#include "WiFiProvisioning.h"
#include "config.h"

IRHandler irHandler(IR_RECV_PIN, IR_LED_PIN);
MQTTHandler mqttHandler(MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD);
WiFiProvisioning* wiFiProvisioning;

void setup() {
  Serial.begin(115200);
  irHandler.setupIR();
  wiFiProvisioning = new WiFiProvisioning("remote_hub");
  wiFiProvisioning->begin();
}

void loop() {
  irHandler.handleIR();
  // mqttHandler.connectMQTT();
  wiFiProvisioning->loop();

  if (Serial.available()) {
    if (Serial.read() == 'p') {
      wiFiProvisioning->startScan();
    }
  }
}