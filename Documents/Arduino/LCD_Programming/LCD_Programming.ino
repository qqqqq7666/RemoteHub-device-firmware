#include <LiquidCrystal.h>
#include <string.h>

LiquidCrystal lcd(22, 23, 24, 25, 26, 27);

String kf = "KOREA Fighting";
String iu = "Induk University";
String ch;

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  lcd.print("induk, mecha!");
}

void loop() {
  if (Serial.available()) {
    delay(100);
    lcd.clear();
    ch = Serial.readString();
    if (ch == kf) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(kf);
    }
    if (ch == iu) {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(iu);
    }
  }
}
