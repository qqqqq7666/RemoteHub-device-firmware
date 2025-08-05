String query = "GET /wid/queryDFSRSS.jsp?zone=1132068100 HTTP/1.1\r\nHost: www.kma.go.kr\r\nConnection: close\r\n\r\n";
const char* sky[4] = { "sunny", "cloudy", "rainny", "rainy" };
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("hi");
}

int temperature, pop, skycode;

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '0') {
      Serial1.println("AT");
    } else if (c == '1') {
      Serial1.println("AT+CWJAP=\"SK_WiFiGIGAE3E8_2.4G\",\"CCY3B@7401\"");
    } else if (c == '2') {
      Serial1.println("AT+CWQAP");
    } else if (c == '3') {
      Serial1.println("AT+CIPSTART=\"TCP\",\"www.kma.go.kr\",80");
    } else if (c == '4') {
      Serial1.println("AT+CIPSEND=" + String(query.length()));
    } else if (c == '5') {
      Serial1.println(query);
      
    }
  }
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
}