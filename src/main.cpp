#include <Arduino.h>

#include "IRHandler.h"
#include "MQTTHandler.h"
#include "WiFiProvisioning.h"
#include "config.h"

IRHandler irHandler(IR_RECV_PIN, IR_LED_PIN);
MQTTHandler mqttHandler(SERVER_IP, MQTT_PORT);
WiFiProvisioning* wiFiProvisioning;

void setup() {
  Serial.begin(115200);
  irHandler.setupIR();
  wiFiProvisioning = new WiFiProvisioning("remote_hub");
  wiFiProvisioning->begin();
}

void loop() {
  irHandler.handleIR();
  mqttHandler.connectMQTT();
  if(Serial.available()) {
    char command = Serial.read();
    if(command == 'u') {
      char testPayload[10] = "justTest";
      mqttHandler.publish("device/test", "justTest");
    }
  }
  // wiFiProvisioning->loop();
}