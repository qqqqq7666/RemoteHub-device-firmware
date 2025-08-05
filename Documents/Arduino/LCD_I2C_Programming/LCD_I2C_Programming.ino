#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>
#define MAX 8

const int ROW_NUM = 4;
const int COLUMN_NUM = 4;
LiquidCrystal_I2C lcd(32, 16, 2);
int a[MAX];
int i = 0;
int x, y, z;

char keys[ROW_NUM][COLUMN_NUM] = {
  { '1', '2', '3', 'X' },
  { '4', '5', '6', '+' },
  { '7', '8', '9', '-' },
  { '*', '0', '=', '/' }
};

byte pin_rows[ROW_NUM] = { 9, 8, 7, 6 };
byte pin_column[COLUMN_NUM] = { 5, 4, 3, 2 };

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

void setup() {

  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    delay(100);
    Serial.print(key);
    Serial.print(" ");
    lcd.write(key);
    a[i] = key - 48;
    i++;
    if (key < 48 || key > 57) {
      --i;
      a[i] = 0;
      i++;
    }
    if (key == 61) {
      for (int j = 0; j < i; j++) {
        z += a[j];
      }
      Serial.print(z);
      lcd.write(z + 48);
      z = 0;
      a[100] = {
        0,
      };
    }
  }
}

int X(int x[]) {
  int a = 0;
  int cnt = 0;
  int result = 0;
  for (int i = 0; i < 10; i++) {
    if (x[i] == NULL) {
      break;
    }
    cnt++;
  }
  cnt -= 1;
  for (int i = cnt; i >= 0; i--) {
    result += x[a] * pow(10, i);
    cnt--;
    a++;
  }
  a = 0;
  cnt = 0;
  return result;
}