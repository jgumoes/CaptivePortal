#include <ESP8266WebServer.h>
#include <webserverInfo.h>

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
  WiFi.disconnect(); // i'm still on the fence as to whether to include this
  WiFi.begin(ssid, password);
  int connRes = WiFi.waitForConnectResult(connAttemptTimeout);
  if (connRes != 3){
    // this little statement is the culmination of an entire day of swearing. it is very important.
    WiFi.disconnect();
  }
  // WebServerData.currentNetwork = WiFi.SSID();
  WebServerData.updateNetwork(WiFi.SSID(), password);
  lastConn.ssid = ssid; lastConn.password = password; lastConn.lastConnRes = connRes; // store results and credentials
  return connRes;
}

int connectWifi(String& ssid, String& password) {
  Serial.println("Connecting as wifi client...");
  Serial.print("Wifi status:\t");
  Serial.println(WiFi.status());
  // if(WiFi.status() == 3){ return 3; } // this guards against a good password being overwritten by a bad password
  int connRes = attemptConnection(ssid, password);
  Serial.print("connRes: ");
  Serial.println(connRes);
  Serial.print("ssid:\t");
  Serial.println(WebServerData.currentNetwork);
  return connRes;
}