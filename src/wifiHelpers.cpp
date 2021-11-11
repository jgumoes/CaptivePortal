#include <ESP8266WebServer.h>
#include <webserverInfo.h>

u_int connAttemptTimeout = 30000; // 20 seconds is enough to get a response from my janky ass-router

class lastConnectionResult {
  // stores the previous connection results
  // TOP SECRET: nobody outside of this file is to know about this
  public:
    String ssid = "";
    String password = "";
    int lastConnRes;

    /* Attempts to save last connection to storage. Sets lastConnRes to NETWORK_STORAGE_FULL
    * if storage is full
    */
    void attemptSave(){
      if(WebServerData.updateNetwork(ssid, password)){
        lastConnRes = WL_CONNECTED;
      }
      else{
        lastConnRes = NETWORK_STORAGE_FULL;
      }
    }

    /* Prepares and returns a response JSON based on lastConnRes
    */
    String prepareResponseObject(){
      Serial.println("prepareResponseObject");
      Serial.println(lastConnRes);
      auto makeResponseObj  = [](String message){
        return "{\"error\": \"" + message + "\", \"connectedNetwork\": \"" + WiFi.SSID() + "\"}";
      };

      if (lastConnRes == NETWORK_STORAGE_FULL){
        // if the maximum number of networks are already stored
        // return makeResponseObj("storageFull"); // todo: put the stored networks in the response object
        return WebServerData.storageFullResponseObj(WiFi.SSID());
      }
      switch(lastConnRes){
        case WL_CONNECTED:
          return makeResponseObj("none");
        case WL_NO_SSID_AVAIL:
          return makeResponseObj("networkLost");
        case WL_WRONG_PASSWORD:
          return makeResponseObj("wrongPass");
        case WL_CONNECT_FAILED:
          return makeResponseObj("wrongPass");
        case -1:  // timeout is presumed due to a wrong password
          return makeResponseObj("wrongPass");
        default:
          return makeResponseObj("other");
      }
    }
} lastConn;

// functions below are the interface for the class above

int attemptConnection(String& newSSID, String& newPassword){
  Serial.print("status: ");
  Serial.println(WiFi.status());
  // catch identical repeat requests.
  int previousResult = lastConn.lastConnRes;
  if (lastConn.ssid == newSSID && lastConn.password == newPassword && (previousResult == 3 || previousResult == 4 || previousResult == NETWORK_STORAGE_FULL || previousResult == 6 )){
    Serial.println("credentials already attempted");
    return lastConn.lastConnRes;
  }
  WiFi.disconnect();
  WiFi.begin(newSSID, newPassword);
  int connRes = WiFi.waitForConnectResult(connAttemptTimeout);

  // store last connection result and credentials
  lastConn.ssid = newSSID; lastConn.password = newPassword; lastConn.lastConnRes = connRes;
  if (connRes == 3){
    lastConn.attemptSave();
  }
  return lastConn.lastConnRes;
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
  int connRes = attemptConnection(ssid, password);
  Serial.print("connRes: ");
  Serial.println(connRes);
  Serial.print("ssid:\t");
  Serial.println(WebServerData.currentNetwork);
  Serial.print("lastConnRes"); Serial.println(lastConn.lastConnRes);

  return lastConn.prepareResponseObject();
}

String forgetWifi(String& ssidList){
  Serial.println("forgetAndConnectWifi()");
  Serial.println(lastConn.lastConnRes);
  if (lastConn.lastConnRes == NETWORK_STORAGE_FULL){
    lastConn.lastConnRes = WL_IDLE_STATUS; // this is to circumvent the catch in attemptConnection
  }
  WebServerData.removeNetworks(ssidList);
  // int connRes = attemptConnection(lastConn.ssid, lastConn.password);
  lastConn.attemptSave();
  Serial.print("connRes: ");
  Serial.println(lastConn.lastConnRes);
  Serial.print("ssid:\t");
  Serial.println(WebServerData.currentNetwork);
  Serial.print("lastConnRes"); Serial.println(lastConn.lastConnRes);
  return lastConn.prepareResponseObject();
}
