#include <SoftwareSerial.h>
#define esp01 Serial1

const String ssid_AP = "ESP01";    // your network SSID (name)
const String pass_AP = "12345678"; //"1234test";         // your network password

char ssid[21] = "SK_WiFiGIGAE3E8_2.4G";    // "SK_WiFiGIGA40F7" your network SSID (name)
char pass[21] = "CCY3B@7401";  // "1712037218" your network password 

String ip = "";
String ap = "";

void ipCheck(String response) {
  int st, ed;
  if (response.indexOf(F("PIP")) != -1) {
    st = response.indexOf(F("PIP"))+5;
    ed = response.indexOf('"', st+1);
    ap = response.substring(st, ed);
    response.remove(0, ed+1);
    Serial.print(F("ap: ")); Serial.println(ap);
  }
  if (response.indexOf(F("AIP")) != -1) {
    st = response.indexOf(F("AIP"))+5;
    ed = response.indexOf('"', st+1);
    ip = response.substring(st, ed);
    Serial.print(F("ip: ")); Serial.println(ip);
  }
}

uint8_t retry = 1;

String sendData(String order) {
  String command = F("AT+");
  command += order;
  command += F("\r\n");
  esp01.print(command); // send the read character to the esp01
  unsigned long int time = millis();
  String response = "";
  while( (time+20000) > millis()) {
    while(esp01.available()) {  // The esp has data so display its output to the serial window
      char c = esp01.read(); // read the next character.
      response+=c;
    }
    if (response.indexOf(F("OK")) != -1) { retry = 0; Serial.println(F("OK")); break; } 
    else if (response.indexOf(F("FAIL")) != -1 || response.indexOf(F("ERROR")) != -1) { 
      if (response.indexOf(F("FAIL")) != -1) retry++;
      else if (response.indexOf(F("ERROR")) != -1) { Serial.println(F("ERROR")); break; } 
      Serial.print(command); Serial.println(F("Failed")); break; 
    }
  }
  return response;
}

String addID(String order, String ssid_AP, String pass_AP) {
  String temp = order+'"'+ssid_AP+'"'+','+'"'+pass_AP+'"';
  return temp;
}

void setup() {
  Serial.begin(9600);
  esp01.begin(9600); // your esp's baud rate might be different
  delay(1000);
  sendData(F("CWQIF")); // disconnect AP if connected
  sendData(F("CWQAP")); // disconnect wifi if connected
  sendData(F("CWMODE=3")); // configure as access point (working mode: AP+STA)
  sendData(F("CWDHCP=1,1"));
  sendData(addID(F("CWSAP="),ssid_AP,pass_AP)+','+String(10)+','+String(3)); 
  retry = 1;
  while(retry) { 
    sendData(addID(F("CWJAP="),ssid,pass)); 
    if (retry > 1) { Serial.print(F("Retrying to connect: ")); Serial.println(retry-1); }
    if (retry > 4) { Serial.println(F("connection failed")); retry = 0; }
  }
  sendData(F("CIPMUX=1")); // configure for multiple connections
  sendData(F("CIPSERVER=1,80")); // turn on server on port 80
  ipCheck(sendData(F("CIFSR"))); // // get ip address
}

//http://apis.data.go.kr/1360000/VilageFcstInfoService_2.0/getVilageFcst?serviceKey=
//A2igtzhW%2ByXkcL7kFKRlaT81NdvwkihPLD1iz0%2BJcNeR2anoHCimnX8xH%2BMbgeRGBOKwfzGS8ChZ88rfKG5%2B6w%3D%3D
//&numOfRows=12&pageNo=1&dataType=JSON&base_date=20230201&base_time=0800&nx=59&ny=125
const char* host = "apis.data.go.kr";
const char KOR_HEAD[] PROGMEM       = "http://apis.data.go.kr/1360000/VilageFcstInfoService_2.0/";
const char KOR_GUSN[] PROGMEM       = "getUltraSrtNcst"; // 초단기실황조회
const char KOR_GUSF[] PROGMEM       = "getUltraSrtFcst"; // 초단기예보조회
const char KOR_GVLF[] PROGMEM       = "getVilageFcst";   // 단기예보조회
const char KOR_ADD0[] PROGMEM       = "?serviceKey=";
const char KOR_KEY[] PROGMEM        = "3ktbY6%2F5rxDdBwvAz%2FPgK%2B9AzbZ%2FFqc7vY5izP1AJlejf%2Fsc1a51ULpy0ZG9a5aFSEQlARtKTzSkesvr%2FVmwbQ%3D%3D"; // 할당받은 API KEY 수정 필요
const char KOR_ADD1[] PROGMEM       = "&numOfRows=";
const char KOR_ADD2[] PROGMEM       = "&pageNo=";
const char KOR_ADD3[] PROGMEM       = "&dataType=JSON&base_date=";
const char KOR_ADD4[] PROGMEM       = "&base_time=";
const char KOR_ADD5[] PROGMEM       = "&nx=";
const char KOR_ADD6[] PROGMEM       = "&ny=";

#define PI 3.1415926535897932384626433832795

int nx, ny;

// 예보 기준 시간: 2, 5, 8, 11, 14, 17, 20, 23 
float lat = 37.49;  // 위도 
float lon = 126.91; // 경도
uint8_t checkTime = 2; 
uint32_t dateTime = 20231108; // 최근 3일간의 자료만 제공
uint8_t Rows = 12; 
uint8_t pageNum = 1;
uint8_t totalPage = 23;
bool getNext = true; 

void getGride(float lat, float lon ) {
  float ra;
  float theta;
  float RE = 6371.00877;   // 지구 반경(km)
  float GRID = 5.0;        // 격자 간격(km)
  float SLAT1 = 30.0;      // 투영 위도1(degree)
  float SLAT2 = 60.0;      // 투영 위도2(degree)
  float OLON = 126.0;      // 기준점 경도(degree)
  float OLAT = 38.0;       // 기준점 위도(degree)
  float XO = 43;           // 기준점 X좌표(GRID)
  float YO = 136;          // 기1준점 Y좌표(GRID)
  float DEGRAD = PI / 180.0;
  float RADDEG = 180.0 / PI;
  float re = RE / GRID;
  float slat1 = SLAT1 * DEGRAD;
  float slat2 = SLAT2 * DEGRAD;
  float olon = OLON  * DEGRAD;
  float olat = OLAT  * DEGRAD;
  float sn = tan( PI * 0.25f + slat2 * 0.5f ) / tan( PI * 0.25f + slat1 * 0.5f );
  sn = log(cos(slat1) / cos(slat2)) / log(sn);
  float sf = tan(PI * 0.25f + slat1 * 0.5f);
  sf = pow(sf, sn) * cos(slat1) / sn;
  float ro = tan(PI * 0.25f + olat * 0.5f);
  ro = re * sf / pow(ro, sn);
  ra = tan(PI * 0.25f + (lat) * DEGRAD * 0.5f);
  ra = re * sf / pow(ra, sn);
  theta = lon * DEGRAD - olon;
  if(theta > PI) theta -= 2.0f * PI;
  if(theta < -PI) theta += 2.0f * PI;
  theta *= sn;
  nx = int(floor(ra * sin(theta) + XO + 0.5f));
  ny = int(floor(ro - ra * cos(theta) + YO + 0.5f));
  Serial.print(F("nx: ")); Serial.println(nx);
  Serial.print(F("ny: ")); Serial.println(ny);
}



String convertTime(uint8_t checkTime) {
  String temp;
  if (checkTime < 10) { temp += '0'; temp += checkTime; }
  else temp += checkTime;
  temp += F("00");
  return temp;
}

bool finished = true;
uint8_t count = 0;
uint8_t connectionID = 3;
uint16_t len = 0;
bool startData = false;
uint8_t failCount = 0;

void get_weather(uint32_t date, String bsTime, uint8_t Rows, uint8_t pageNum) {
  Serial.println(F("Starting connection to server..."));
  startData = false;
  bool connectOK = false;
  String temp;
  esp01.print(F("AT+CIPSTART="));
  esp01.print(connectionID);
  esp01.print(F(",\"TCP\","));
  esp01.print('"');
  esp01.print(host);
  esp01.print('"');
  esp01.print(F(",80\r\n"));
  unsigned long int time = millis();
  while (time+10000 > millis()) {
    temp = esp01.readStringUntil('\n');
    if (temp.indexOf(F("OK")) != -1) { connectOK = true; break; }
    else if (temp.indexOf(F("ERROR")) != -1) { Serial.println(F("Connection Failed")); break; }
  }
  while (esp01.available()) char c = esp01.read();
  if (connectOK) {
    Serial.println(F("Connected to server"));
    temp = F("GET ");
    temp += (const __FlashStringHelper *)KOR_HEAD;
    temp += (const __FlashStringHelper *)KOR_GVLF;
    temp += (const __FlashStringHelper *)KOR_ADD0;
    temp += (const __FlashStringHelper *)KOR_KEY;
    temp += (const __FlashStringHelper *)KOR_ADD1;
    temp += Rows;
    temp += (const __FlashStringHelper *)KOR_ADD2;
    temp += pageNum;
    temp += (const __FlashStringHelper *)KOR_ADD3;
    temp += date;
    temp += (const __FlashStringHelper *)KOR_ADD4;
    temp += bsTime;
    temp += (const __FlashStringHelper *)KOR_ADD5;
    temp += nx;
    temp += (const __FlashStringHelper *)KOR_ADD6;
    temp += ny;
    temp += F(" HTTP/1.1\r\nHost: ");
    temp += host;
    temp += F("\r\nConnection: close\r\n\r\n");
    esp01.print(F("AT+CIPSEND="));
    esp01.print(connectionID);
    esp01.print(',');
    esp01.print(temp.length());
    esp01.print(F("\r\n"));
    if(esp01.find('>')) {
      Serial.print(">");
      esp01.print(temp);
      unsigned long int time = millis();
      while (time+10000 > millis()) {
        temp = esp01.readStringUntil('\n');
        if (temp.indexOf(F("SEND OK")) != -1) { Serial.println(F("request OK")); } //startData = true; 
        else if (temp.indexOf(F("ERROR")) != -1) { Serial.println(F("request Failed")); break; }
        if (temp.indexOf(F("200 OK")) != -1) { 
          temp = temp.substring(temp.lastIndexOf(',')+1, temp.lastIndexOf(':'));
          len = temp.toInt()-14; // -15(HTTP/1.1 200 OK) +1(continue check: +IPD)
          startData = true;
          Serial.println(F("start Data")); break; 
        }
      } 
    }
    getNext = !startData;
  } else { 
    failCount++; delay(500); 
    Serial.println(failCount);
    if (failCount > 5) {
      Serial.println(F("Failed & Finished the task")); 
      failCount = 0; finished = true; count = 0; pageNum = 1; getNext = true; 
    }
  }
}

bool checkRain = false;

void loop() {
  if (!finished) {
    while (count < totalPage) {
      if (getNext) {
        if (count == 0) getGride(lat, lon); // 위도, 경도
        String cTime = convertTime(checkTime);
        Serial.println(F("--------------------"));
        Serial.print(dateTime); Serial.print('/'); Serial.println(cTime);  
        Serial.print(Rows);Serial.print('/');Serial.println(pageNum);
        get_weather(dateTime, cTime, Rows, pageNum);
      }
      if (esp01.available() && startData) { 
        Serial.println(F("OK"));
        bool check = false; char cc; 
        while (len) {  
          if (esp01.available()) {
            cc = esp01.read(); len--;
            if (cc == '+') {
              String tt;
              check = esp01.find(',');
              check = esp01.find(',');
              while (cc != ':') { if (esp01.available()) { cc = esp01.read(); if(cc != ':') tt += cc; } }
              len = tt.toInt()+1; // +1: 추가로 받아야 하는 데이터가 있는지 확인
            } else {
              Serial.print(cc);
              if (cc == '}') Serial.println();
            }
          }
        }
        delay(1);
        while (esp01.available()) cc = esp01.read();
        count++; getNext = true; pageNum++; failCount = 0;
        if (count == totalPage) { 
          finished = true; 
        }
      }
      if (finished) break;
    }
    count = 0; pageNum = 1; 
  }
  if(Serial.available() > 0){
    String temp = Serial.readStringUntil('\n');
    Serial.println(temp);
    if (temp == "1") {  // 시리얼 모니터에서 날씨정보 요청 명령
      finished = !finished;
    }
  }
}
