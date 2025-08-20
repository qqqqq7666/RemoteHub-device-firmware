#include "IRHandler.h"

IRHandler::IRHandler(uint16_t recvPin, uint16_t sendPin) : irrecv(recvPin), irsend(sendPin), lastReceivedValue(0) {}

void IRHandler::setupIR() {
  irrecv.enableIRIn();  // Start the IR receiver
  irsend.begin();       // Start the IR sender
  Serial.println("KY-022 Infrared Receiver Test");
}

void IRHandler::handleIR() {
  if (irrecv.decode(&results)) {
    Serial.printf("Received IR Code: 0x%X\n", results.value);
    if (results.decode_type != UNKNOWN && results.decode_type != UNUSED && results.value < 0xFFFFFFFF) {
      lastReceivedValue = results.value;
    }
    irrecv.resume();  // Receive next value
  }

  if (Serial.available()) {
    char command = Serial.read();
    if (command == 's') {
      Serial.printf("Sending IR Code: 0x%X\n", lastReceivedValue);
      irsend.sendNEC(lastReceivedValue);  // Send the last received value
    } else if (command == 'r') {
      Serial.println("Resetting last received value.");
      lastReceivedValue = 0;  // Reset the last received value
    }
  }

  delay(500);
}

void IRHandler::send(uint32_t signal) {
  irsend.sendNEC(signal);
}

uint32_t IRHandler::getLastReceiveSignal() {
  return lastReceivedValue;
}

int IRHandler::getDecodeType(uint32_t signal) {
  return results.decode_type;
}