#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

const char* SERVER_IP = "192.168.45.18";
const int MQTT_PORT = 1883;
const char* MQTT_USER = "user";
const char* MQTT_PASSWORD = "";

const char* MQTT_ID = "remoteHubId123";
const char* REMOTE_HUB_PASSWORD = "123";

const uint16_t IR_LED_PIN = 27;   // sender 27
const uint16_t IR_RECV_PIN = 32;  // receiver 32

#endif