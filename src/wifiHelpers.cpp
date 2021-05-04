#include <ESP8266WebServer.h>

bool connectWifi(String& ssid, String& password) {
  Serial.println("Connecting as wifi client...");
  // WiFi.disconnect();
  WiFi.begin(ssid, password);
  int connRes = WiFi.waitForConnectResult(10000);
  if (connRes != 3){
    // this little statement is the culmination of an entire day of swearing. it is very important.
    WiFi.disconnect();
  }
  Serial.print("connRes: ");
  Serial.println(connRes);
  Serial.print("status: ");
  Serial.println(WiFi.status());
  return connRes == 3 ? true : false;
}