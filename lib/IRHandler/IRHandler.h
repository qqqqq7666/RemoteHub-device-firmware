#ifndef IRHANDLER_H
#define IRHANDLER_H

#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

class IRHandler {
 public:
  IRHandler(uint16_t recvPin, uint16_t sendPin);

  void setupIR();
  void handleIR();

 private:
  IRrecv irrecv;
  IRsend irsend;
  decode_results results;
  uint32_t lastReceivedValue;
};

#endif