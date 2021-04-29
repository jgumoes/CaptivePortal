#include <ESP8266WebServer.h>

bool connectWifi(String& ssid, String& password) {
  Serial.println("Connecting as wifi client...");
  WiFi.disconnect();
  Serial.println(millis());
  WiFi.begin(ssid, password);
  Serial.println(millis());
  int connRes = WiFi.waitForConnectResult();
  Serial.println(millis());
  Serial.print("connRes: ");
  Serial.println(connRes);
  return true;
}