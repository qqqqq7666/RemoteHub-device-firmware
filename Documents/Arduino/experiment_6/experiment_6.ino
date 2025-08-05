void setup() {
  DDRA = 0x0f;
  unsigned char led1 = 0x01;
  unsigned char led2 = 0x08;
  int i;
  int j;
  while (1) {
    if ((PINA & 0x10) == 0x10) {
      for (i = 1; i <= 3; i++) {
        for (j = 1; j <= 4; j++) {
          PORTA = led1;
          _delay_ms(1000);
          led1 <<= 1;
          if (led1 == 0x10) {
            led1 = 0x01;
          }
        }
        PORTA = 0x00;
      }
    }
    if ((PINA & 0x20) == 0x20) {
      for (i = 1; i <= 2; i++) {
        for (j = 1; j <= 4; j++) {
          PORTA = led2;
          _delay_ms(1000);
          led2 >>= 1;
          if (led2 == 0x00) {
            led2 = 0x10;
          }
        }
        PORTA = 0x00;
      }
    }
    if ((PINA & 0x40) == 0x40) {
      for (i = 1; i <= 3; i++) {
        for (j = 1; j <= 4; j++) {
          PORTA = led1;
          _delay_ms(1000);
          led1 <<= 1;
          led1 |= 0x01;
          if (led1 == 0x1f) {
            PORTA=0x00;
            _delay_ms(1000);
            led1 = 0x01;
          }
        }
        PORTA = 0X00;
      }
    }
    if ((PINA & 0x80) == 0x80) {
      for (i = 1; i <= 2; i++) {
        for (j = 1; j <= 4; j++) {
          PORTA = led2;
          _delay_ms(1000);
          led2 >>= 1;
          led2 |= 0x08;
          if (led2 == 0x0f) {
            PORTA =0x0f;
            _delay_ms(1000);
            PORTA = 0x00;
            _delay_ms(1000);
            led2 = 0x10;
          }
        }
      }
    }
  }
}
void loop() {
}
