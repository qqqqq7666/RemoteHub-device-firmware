#include <HTTPClient.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <WebServer.h>
#include <WiFi.h>

const char *ssidStr = "SK_WiFiGIGAE3E8_2.4G";
const char *passwordStr = "";
const char *ssid = "ESP32_AP";
const char *password = "12345678";

// SoftAP에서는 고정 IP 192.168.4.1이 자동 설정됨
WebServer server(80);  // 80번 포트로 HTTP 웹서버 실행
HTTPClient http;       // HTTP 클라이언트 객체

const uint16_t irLedPin = 27;   // sender 27
const uint16_t irRecvPin = 32;  // receiver 32

IRrecv irrecv(irRecvPin);
IRsend irsend(irLedPin);
decode_results results;

uint32_t lastReceivedValue = 0;

void handleRoot() {
  int n = WiFi.scanNetworks();

  String html = "<html><body>";
  html += "<h2>Select Wi-Fi</h2>";
  html += "<form action='/connect' method='POST'>";
  html += "<label for='ssid'>SSID:</label><select name='ssid'>";

  for (int i = 0; i < n; ++i) html += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";

  html += "</select><br>";
  html += "<label for='password'>Password:</label>";
  html += "<input type='password' name='password' placeholder='Enter password'><br>";
  html += "<input type='submit' value='Connect'>";
  html += "</form></body></html>";

  server.send(200, "text/html", html);
}

void handleConnect() {
  if (server.hasArg("ssid") && server.hasArg("password")) {
    String inputSsid = server.arg("ssid");
    String inputPassword = server.arg("password");
    const char *clientSsid = inputSsid.c_str();
    const char *clientPassword = inputPassword.c_str();

    Serial.printf("Trying to connect to SSID: %s\n", clientSsid);

    Serial.printf("ssid: %s\n", clientSsid);
    Serial.printf("password: %s\n", clientPassword);
    Serial.printf("ssid length: %d\n", strlen(clientSsid));

    WiFi.begin(clientSsid, clientPassword);

    unsigned long startAttemptTime = millis();

    // 10초 동안 연결 대기
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
      delay(500);
      Serial.print(".");
      wl_status_t wifiStatus = WiFi.status();
      switch (wifiStatus) {
        case WL_IDLE_STATUS:
          Serial.println(" -> IDLE");
          break;
        case WL_NO_SSID_AVAIL:
          Serial.println(" -> SSID not available");
          break;
        case WL_SCAN_COMPLETED:
          Serial.println(" -> Scan completed");
          break;
        case WL_CONNECT_FAILED:
          Serial.println(" -> Connection failed");
          break;
        case WL_CONNECTION_LOST:
          Serial.println(" -> Connection lost");
          break;
        case WL_DISCONNECTED:
          Serial.println(" -> Disconnected");
          break;
        case WL_CONNECTED:
          Serial.println(" -> Connected");
          break;
      }
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWi-Fi Connected!");
      server.send(200, "text/html", "<h2>Wi-Fi Connected!</h2><a href='/call-api'>Call Test API</a>");
    } else {
      Serial.println("\nConnection Failed");
      server.send(200, "text/html", "<h2>Wi-Fi Connection Failed</h2><a href='/'>Try Again</a>");
    }
  } else {
    server.send(400, "text/plain", "Missing ssid or password");
  }
}

void handleCallApi() {
  if (WiFi.status() == WL_CONNECTED) {
    String apiURL = "http://192.168.45.19:8080/api/v1/users/test";  // 여기에 본인 API 주소 넣기
    http.begin(apiURL);
    int httpCode = http.GET();

    String response = "";
    if (httpCode > 0) {
      response = http.getString();
      Serial.println("API Response:");
      Serial.println(response);
      server.send(200, "text/html", "<h2>API call Success</h2><pre>" + response + "</pre>");
    } else {
      server.send(200, "text/html", "<h2>API call Failed</h2>");
    }

    http.end();
  } else {
    server.send(200, "text/html", "<h2>Wi-Fi Not Connected</h2>");
  }
}

void callTestAPI() {
  String apiURL = "http://localhost/api/v1/users/test";
  http.begin(apiURL);
  int httpResponseCode = http.GET();
  String response = http.getString();
}

void checkConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi not connected, attempting to reconnect...");
  } else {
    Serial.println("Wi-Fi is connected.");
    Serial.printf("SSID: %s\n", WiFi.SSID().c_str());
  }
}

void connectMyOwnWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  // WiFi에 연결이되고 DHCP로 IP를 받아오면 WiFi.localIP()확인할 수 있다.
  Serial.println(WiFi.localIP());
}

void disconnectWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFi.disconnect();
    Serial.println("Wi-Fi disconnected.");
  } else {
    Serial.println("Wi-Fi is not connected.");
  }
}

void handleIR() {
  if (irrecv.decode(&results)) {
    Serial.printf("Received IR Code: 0x%X\n", results.value);
    if (results.decode_type != UNKNOWN && results.decode_type != UNUSED && results.value < 0xFFFFFFFF) lastReceivedValue = results.value;
    irrecv.resume();  // Receive next value
  }
  if (Serial.available()) {
    char command = Serial.read();
    if (command == 's') {
      Serial.printf("Sending IR Code: 0x%X\n", lastReceivedValue);
      irsend.sendNEC(lastReceivedValue);  // Send the last received value
    } else if (command == 'r') {
      Serial.println("Resetting last received value.");
      lastReceivedValue = 0;  // Reset the last received value
    }
  }
  delay(500);
}

void setup() {
  Serial.begin(115200);

  // WiFi를 AP_STA 모드로 설정한다.
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);

  Serial.print("Connecting to ");
  Serial.println(ssidStr);

  // WiFi 접속을 시작한다.
  WiFi.begin(ssidStr, passwordStr);

  server.on("/", handleRoot);
  server.on("/connect", HTTP_POST, handleConnect);
  server.on("/call-api", handleCallApi);
  server.on("/check-connection", checkConnection);
  server.on("/connect-my-own-wifi", connectMyOwnWiFi);
  server.on("/disconnect-wifi", disconnectWiFi);

  server.begin();
  Serial.println("Web server started");

  if (WiFi.status() == WL_CONNECTED)
    Serial.println("\nWi-Fi Connected!");
  else
    Serial.println("\nConnection Failed");

  irrecv.enableIRIn();  // Start the IR receiver
  irsend.begin();       // Start the IR sender

  Serial.println("KY-022 Infrared Receiver Test");
}

void loop() {
  server.handleClient();
  handleIR();
}