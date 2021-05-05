#include <ESP8266WebServer.h>

struct lastConnectionResult {
  String ssid = "";
  String password = "";
  int lastConnRes;
} lastConn;

int attemptConnection(String& ssid, String& password){
  if (lastConn.ssid == ssid && lastConn.password == password){
    Serial.println("credentials already attempted");
    return lastConn.lastConnRes;
  }
  // WiFi.disconnect();
  WiFi.begin(ssid, password);
  int connRes = WiFi.waitForConnectResult(10000);
  if (connRes != 3){
    // this little statement is the culmination of an entire day of swearing. it is very important.
    WiFi.disconnect();
  }
  lastConn.ssid = ssid; lastConn.password = password; lastConn.lastConnRes = connRes;
  return connRes;
}

bool connectWifi(String& ssid, String& password) {
  Serial.println("Connecting as wifi client...");
  int connRes = attemptConnection(ssid, password);
  Serial.print("connRes: ");
  Serial.println(connRes);
  Serial.print("status: ");
  Serial.println(WiFi.status());
  return connRes == 3 ? true : false;
}