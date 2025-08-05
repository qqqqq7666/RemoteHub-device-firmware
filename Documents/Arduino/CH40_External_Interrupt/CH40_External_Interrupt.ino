#include <Wire.h>
#include <LiquidCrystal_I2C.h>

volatile int count0 = 0, count1 = 0;
const byte interruptPin0 = 2, interruptPin1 = 3;
int led_state = 16;

LiquidCrystal_I2C lcd(32, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup() {
  noInterrupts();
  DDRB = 0xff;
  DDRC = 0x00;
  interrupts();
  attachInterrupt(digitalPinToInterrupt(interruptPin0), cnt0, FALLING);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), cnt1, RISING);

  lcd.init();
  lcd.backlight();
}

void loop() {
  PORTC = 0x01;
  if (count0 > 4 || count1 > 4) {
    count0 = 0;
    count1 = 0;
    PORTB = 0x00;
    led_state = 16;
  }
  if (count0 > 0 && count1 == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Falling Edge");
    lcd.setCursor(0, 1);
    lcd.print(count0);
    if (count0 > 4) {
      lcd.setCursor(0, 1);
      lcd.print(0);
    }
  }
  if (count1 > 0 && count0 == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Rising Edge");
    lcd.setCursor(0, 1);
    lcd.print(count1);
    if (count1 > 4) {
      lcd.setCursor(0, 1);
      lcd.print(0);
    }
  }
}

void cnt0() {
  if (count1 == 0) {
    count0++;
    PORTB <<= 1;
    PORTB |= 1;
    PORTC = 0x00;
  }
}

void cnt1() {
  if (count0 == 0) {
    count1++;
    led_state >>= 1;
    PORTB = led_state;
  }
}