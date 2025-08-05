#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* int seg[7] = { 3,4,5,6,7,8,9 };
  { 1, 1, 1, 1, 1, 1, 0 },  //0
  { 0, 1, 1, 0, 0, 0, 0 },  //1
  { 1, 1, 0, 1, 1, 0, 1 },  //2
  { 1, 1, 1, 1, 0, 0, 1 },  //3
  { 0, 1, 1, 0, 0, 1, 1 },  //4
  { 1, 0, 1, 1, 0, 1, 1 },  //5
  { 1, 0, 1, 1, 1, 1, 1 },  //6
  { 1, 1, 1, 0, 0, 0, 0 },  //7
  { 1, 1, 1, 1, 1, 1, 1 },  //8
  { 1, 1, 1, 1, 0, 1, 1 }   //9 */
int num[10] = { 248, 48, 216, 120, 48, 104, 232, 56, 248, 120 };
int num2[10] = { 1, 0, 2, 2, 3, 3, 3, 0, 3, 3 };
volatile int state = 0;
unsigned char led = 0x00;
int i;

ISR(INT0_vect) {
  state++;
  led = i;
  PORTB = led * 4;
}

void INIT_PORT() {
  DDRD = 0xf8;
  DDRB = 0xff;
  PORTD = 0xfc;
  PORTB = 0x01;
  Serial.begin(9600);
}

void INIT_INT0() {
  EIMSK |= (1 << INT0);
  EICRA |= (1 << ISC01);
  sei();
}

int main() {
  INIT_PORT();
  if (digitalRead(2) == 0) {
    INIT_INT0();
    while (1) {
      PORTD |= 0x04;
      Serial.print("state = ");
      Serial.println(state);
      Serial.print("sw_value = ");
      Serial.println(digitalRead(2));
      for (i = 0; i < 10; i++) {
        PORTD = num[i];
        PORTB = num2[i];
        _delay_ms(300);
        if (state > 1) {
          state = 0;
          break;
        }
      }
    }
  }
}