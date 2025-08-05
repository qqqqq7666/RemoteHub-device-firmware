#include <RTClib.h>

#include <Wire.h>
RTC_DS1307 rtc;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(__DATE__, __TIME__));
}

void loop() {
  DateTime now = rtc.now();
  now.minute()
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute());
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  if(now.minute() == 22)
    Serial.println("fuck!!!!!!");
  delay(1000);
}
