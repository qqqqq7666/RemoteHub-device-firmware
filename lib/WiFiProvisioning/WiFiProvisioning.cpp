#include "WiFiProvisioning.h"

#define SERVICE_UUID "12345678-1234-1234-1234-1234567890ab"
#define SSID_CHAR_UUID "abcd1234-5678-90ab-cdef-1234567890ab"
#define PASS_CHAR_UUID "abcd5678-1234-90ab-cdef-1234567890ab"
#define SCAN_RESULT_UUID "abcd9999-1234-90ab-cdef-1234567890ab"

WiFiProvisioning::WiFiProvisioning(const std::string &deviceName) { BLEDevice::init(deviceName); }

void WiFiProvisioning::begin() {
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);

  pSSIDChar = pService->createCharacteristic(SSID_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE);

  pPASSChar = pService->createCharacteristic(PASS_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE);

  pScanResultChar = pService->createCharacteristic(SCAN_RESULT_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

  pSSIDChar->setCallbacks(new SSIDCallback(this));
  pPASSChar->setCallbacks(new PASSCallback(this));

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->start();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);  // 기존 연결/캐시 초기화
  delay(100);

  Serial.println("BLE WiFi Provisioning Service started. Waiting for connection...");
  startScan();
}

void WiFiProvisioning::loop() {
  // 필요하면 주기적으로 스캔 결과 업데이트
}

void WiFiProvisioning::startScan() {
  Serial.println("Scanning WiFi networks...");
  int n = WiFi.scanNetworks();
  if (n == 0) {
    pScanResultChar->setValue("No networks found");
  } else {
    String scanList;
    for (int i = 0; i < n; i++) {
      scanList += WiFi.SSID(i);
      if (i < n - 1) scanList += "\n";
    }
    pScanResultChar->setValue(scanList.c_str());
    Serial.println(scanList);
    Serial.printf("Found %d networks\n", n);
  }
}

void WiFiProvisioning::connectToWiFi() {
  Serial.println(WiFi.SSID());
  ssid.replace("\r", "");
  ssid.replace("\n", "");
  password.replace("\r", "");
  password.replace("\n", "");
  Serial.printf("connect to %s\npassword is %s\n", ssid.c_str(), password);
  if (ssid.length() == 0 || password.length() == 0) {
    Serial.println("SSID or password not set yet.");
    return;
  }
  Serial.printf("Connecting to WiFi: %s\n", ssid.c_str());
  WiFi.begin(ssid.c_str(), String(password).c_str());

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 40) {
    delay(500);
    Serial.print(".");
    retry++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi.");
    Serial.println(WiFi.status());
  }
}

// SSID 콜백
void WiFiProvisioning::SSIDCallback::onWrite(BLECharacteristic *pChar) {
  std::string value = pChar->getValue();
  prov_->ssid = String(value.c_str());
  Serial.printf("Received SSID: %s\n", prov_->ssid.c_str());
}

// PASSWORD 콜백
void WiFiProvisioning::PASSCallback::onWrite(BLECharacteristic *pChar) {
  prov_->password = pChar->getValue().c_str();
  Serial.printf("Received Password: %s\n", prov_->password.c_str());
  prov_->connectToWiFi();
}