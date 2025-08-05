int seg[7] = { 5, 6, 9, 8, 7, 4, 3 };

int num[10][7] = {
  { 1, 1, 1, 1, 1, 1, 0 },  //0
  { 0, 1, 1, 0, 0, 0, 0 },  //1
  { 1, 1, 0, 1, 1, 0, 1 },  //2
  { 1, 1, 1, 1, 0, 0, 1 },  //3
  { 0, 1, 1, 0, 0, 1, 1 },  //4
  { 1, 0, 1, 1, 0, 1, 1 },  //5
  { 1, 0, 1, 1, 1, 1, 1 },  //6
  { 1, 1, 1, 0, 0, 0, 0 },  //7
  { 1, 1, 1, 1, 1, 1, 1 },  //8
  { 1, 1, 1, 1, 0, 1, 1 }   //9
};
int number;

int led[4] = { 10, 11, 12, 13 };
int* LED = {
  0,
};
int sw = 2;
int value;
int lednum;
int cnt = 0, sign = 1;
int I = 0, J = 0;

void setup() {
  for (int i = 0; i < 7; i++) {
    pinMode(seg[i], OUTPUT);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(led[i], OUTPUT);
  }
  pinMode(sw, INPUT_PULLUP);
  for (int i = 0; i < 7; i++) {
    digitalWrite(seg[i], num[0][i]);
  }
  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(sw), SW, FALLING);
}

void loop() {
  if (digitalRead(sw) == LOW) {
    cnt = 1;
    Serial.println(cnt);
  }
  if (cnt == 1) {
    Serial.println(value);
    for (I = 0; I < 10; I++) {
      for (J = 0; J < 7; J++) {
        digitalWrite(seg[J], num[I][J]);
      }
      delay(1000);
      if (cnt > 1)
        break;
    }
  }
}

void SW() {
  cnt = 2;
  lednum = I;
  Serial.println(lednum);
  LED = DToB(lednum);
  for (int i = 3; i >= 0; i--) {
    Serial.print(LED[i]);
    Serial.print(" ");
  }
  Serial.println(" ");
  for (int i = 0; i < 4; i++) {
    digitalWrite(led[i], LED[i]);
  }
}

int* DToB(int a) {
  int* result = {
    0,
  };
  int j, b = 0, cnt = 0;
  while (a != 0) {
    result[cnt] = a % 2;
    a /= 2;
    cnt++;
  }
  return result;
}