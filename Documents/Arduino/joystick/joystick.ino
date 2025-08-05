#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
LiquidCrystal lcd1(12, 11, 5, 4, 3, 2);
LiquidCrystal_I2C lcd2(32,16,2);
String str = Serial.readString();

void setup() {
  Wire.begin();
  lcd2.init();
  lcd1.begin(16, 2);
  lcd2.backlight();
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    delay(100);
    while (Serial.available() > 0) {
      lcd1.print(Serial.read());
      if (str == "Induk University") {
        lcd2.clear();
        lcd2.setCursor(0, 0);
        lcd2.print(str);
      }
      if (str == "Good Mecha!!") {
        lcd1.clear();
        lcd1.setCursor(0, 0);
        lcd1.print(str);
      }
    }
  }
}
