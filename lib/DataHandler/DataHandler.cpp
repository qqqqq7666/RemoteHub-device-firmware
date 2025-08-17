// #include "DataHandler.h"

// #include <map>

// DataHandler::DataHandler(String& registerIrUrl, String& retrieveIrUrl) : registerIrUrl(registerIrUrl), retrieveIrUrl(retrieveIrUrl) {}

// void DataHandler::registerSignal(String& signal) {
//   http.begin(registerIrUrl);
//   http.addHeader("Content-Type", "application/json");
//   String body = "{\"signal\":\"" + signal + "\"}";
//   int code = http.POST(body);
//   Serial.println("Register response code: " + String(code));
//   http.end();
// }

// void DataHandler::retrieveSignal() {
//   http.begin(retrieveIrUrl);
//   int code = http.GET();
//   if (code == 200) {
//     String payload = http.getString();
//     Serial.println("Retrieved: " + payload);
//   }
//   http.end();
// }