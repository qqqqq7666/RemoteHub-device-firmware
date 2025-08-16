#include <HTTPClient.h>
#include <MySQL_Generic.h>

class DataHandler {
 private:
  HTTPClient http;
  String registerIrUrl;
  String retrieveIrUrl;

 public:
  DataHandler(String& registerIrUrl, String& retrieveIrUrl);
  void registerSignal(String& signal);
  void retrieveSignal();
};