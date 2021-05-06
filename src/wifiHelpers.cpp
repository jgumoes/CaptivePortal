#include <ESP8266WebServer.h>

u_int connAttemptTimeout = 30000; // 20 seconds is enough to get a response from my janky ass-router

struct lastConnectionResult {
  // stores the previous connection results
  String ssid = "";
  String password = "";
  int lastConnRes;
} lastConn;


int attemptConnection(String& ssid, String& password){
  Serial.print("status: ");
  Serial.println(WiFi.status());
  if (lastConn.ssid == ssid && lastConn.password == password){
    Serial.println("credentials already attempted");
    return lastConn.lastConnRes;
  }
  // WiFi.disconnect(); // i'm still on the fence as to whether to include this
  WiFi.begin(ssid, password);
  int connRes = WiFi.waitForConnectResult(connAttemptTimeout);
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
  return connRes == 3 ? true : false;
}