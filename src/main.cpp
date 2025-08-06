#include <WifiHandler.hpp>
#include <IRHandler.hpp>

WifiHandler wifiHandler;
IRHandler irHandler;

void setup() {
  Serial.begin(115200);
  wifiHandler.setupWiFi();
  irHandler.setupIR();
}

void loop() {
  server.handleClient();
  irHandler.handleIR();
}