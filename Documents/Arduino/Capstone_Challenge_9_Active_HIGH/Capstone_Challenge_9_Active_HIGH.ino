unsigned char seg[] = { 0x7f, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b };
unsigned char led[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 };
int i;
int j = 0;
static int cnt = 0;

void setup() {
  DDRA = 0x0f;
  DDRC = 0xff;
}

void loop() {
  while (1) {
    j++;
    delay(250);
    if (PINA & 0x10 == 0x10) {
      cnt += 1;
      if (cnt = 1) {
        if (PINA & 0x10 == 0x10) {
          for (i = 0; i < 9; i++) {
            PORTC = seg[i];
            _delay_ms(250);
          }
        }
      }
      if (cnt = 2) {
        if (PINA & 0x10 == 0x10) {
          PORTC = seg[j];
          PORTA = led[j];
        }
        cnt = 0;
        j = 0;
      }
    }
  }
}