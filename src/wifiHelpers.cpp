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
  int previousResult = lastConn.lastConnRes;
  if (lastConn.ssid == ssid && lastConn.password == password && (previousResult == 3 || previousResult == 4)){
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

  lastConn.ssid = ssid; lastConn.password = password; lastConn.lastConnRes = connRes; // store results and credentials
  if(WebServerData.updateNetwork(WiFi.SSID(), password) == 0){
    lastConn.lastConnRes = NETWORK_STORAGE_FULL;
  }
  return lastConn.lastConnRes;
}

String prepareResponseObject(int connRes){
  auto makeResponseObj  = [](String message){ return "{\"error\": \"" + message + "\"}";};

  if (connRes == NETWORK_STORAGE_FULL){
    // if the maximum number of networks are already stored
    // return makeResponseObj("storageFull"); // todo: put the stored networks in the response object
    return WebServerData.storageFullResponseObj();
  }
  
  switch(connRes){
    case WL_CONNECTED:
      return makeResponseObj("none");
    case WL_NO_SSID_AVAIL:
      return makeResponseObj("networkLost");
    case WL_CONNECT_FAILED:
      return makeResponseObj("wrongPass");
    case -1:  // timeout is presumed due to a wrong password
      return makeResponseObj("wrongPass");
    default:
      return makeResponseObj("other");
  }
}

/*
 * Attempts to connect to the given wifi network.
 * @returns the connection result to pass on to the webpage.
 * i.e. returns "none" if connected without error;
 *      returns "wrongPass" if the password was wrong or the connection timed out;
 *      returns "storageFull" if the max number of networks has been reached;
 *      returns "other" if the connection failed for any other reason;
 */
String connectWifi(String& ssid, String& password) {
  Serial.println("Connecting as wifi client...");
  Serial.print("Wifi status:\t");
  Serial.println(WiFi.status());
  // if(WiFi.status() == 3){ return 3; } // this guards against a good password being overwritten by a bad password
  int connRes = attemptConnection(ssid, password);
  Serial.print("connRes: ");
  Serial.println(connRes);
  Serial.print("ssid:\t");
  Serial.println(WebServerData.currentNetwork);

  return prepareResponseObject(connRes);
}