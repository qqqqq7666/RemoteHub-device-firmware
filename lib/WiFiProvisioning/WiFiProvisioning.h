#ifndef WIFI_PROVISIONING_H
#define WIFI_PROVISIONING_H

#include <Arduino.h>
#include <WiFi.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

class WiFiProvisioning {
public:
    WiFiProvisioning(const std::string &deviceName);
    void begin();
    void loop();
    void startScan();

private:
    BLEServer *pServer;
    BLEService *pService;
    BLECharacteristic *pSSIDChar;
    BLECharacteristic *pPASSChar;
    BLECharacteristic *pScanResultChar;

    String ssid;
    String password;

    void connectToWiFi();

    class SSIDCallback : public BLECharacteristicCallbacks {
    public:
        SSIDCallback(WiFiProvisioning *prov) : prov_(prov) {}
        void onWrite(BLECharacteristic *pChar) override;
    private:
        WiFiProvisioning *prov_;
    };

    class PASSCallback : public BLECharacteristicCallbacks {
    public:
        PASSCallback(WiFiProvisioning *prov) : prov_(prov) {}
        void onWrite(BLECharacteristic *pChar) override;
    private:
        WiFiProvisioning *prov_;
    };
};

#endif
