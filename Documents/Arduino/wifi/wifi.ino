#include <SoftwareSerial.h>
#define esp01 Serial1

void setup() {
  Serial.begin(9600);
  esp01.begin(9600);
  Serial.println("Hello");
}

void loop() {
  if (esp01.available())
    Serial.write(esp01.read());
  if (Serial.available()) {

    esp01.write(Serial.read());
  }
}
