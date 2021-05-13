#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include "wifiHelpers.h"
#include "webserverInfo.h"

ESP8266WebServer server(80);

void handlePortalClient(){
  server.handleClient();
}

bool handleCaptivePortal() { // send the right file to the client (if it exists)
  const char* path = "/config.html";
  String contentType = "text/html";            // Get the MIME type
  if (LittleFS.exists(path)) {                            // If the file exists
    File file = LittleFS.open(path, "r");                 // Open it
    server.streamFile(file, contentType); // And send it to the client
    // Serial.println("file sent to client");
    file.close();                                       // Then close the file again
    return true;
  }
  // Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}

void sendScanNetworks() {
  int numOfNetworks = WiFi.scanNetworks();
  String ssidList = "{\"network_list\":[";
  for(int i = 0; i < numOfNetworks; i++) {
    ssidList += "\"" + WiFi.SSID(i) + "\"";
    if(numOfNetworks - i != 1) { ssidList += ",";}
  }
  ssidList += "],\"attempted_network\":\"\"}";
  server.send(200, "application/json", ssidList);
}

void handleWifiSave(){
  Serial.println("#########################################");
  Serial.println("wifi save");
  String ssid = server.arg("ssid");
  String password = server.arg("pwd");
  // there should be a check for if a password for the ssid is already stored
  // bool connRes = connectWifi(ssid, password) == 3 ? true : false;
  String responseObj = connectWifi(ssid, password);
  server.send(200, "application/json", responseObj);
  server.client().stop(); // Stop is needed because we sent no content length
  Serial.println("#########################################");
  WebServerData.printStoredNetworks();
}

void handleServerInfo() {
  WebServerData.deviceName();
  Serial.println("handleServerInfo");
  char responseObj[WebServerData.infoResponseSize()];
  WebServerData.getServerInfo(responseObj);
  Serial.print(responseObj);
  server.send(200, "application/json", responseObj);
}


/* Returns the contents of config.json.
 * For development use only.
 */
void handleConfigJson() {
  // server.send(200, "text/json");
  String path = WebServerData.configFileName();
  if (LittleFS.exists(path)) {
    // If the file exists
    File file = LittleFS.open(path, "r");                 // Open it
    server.streamFile(file, "text/json"); // And send it to the client
    // Serial.println("file sent to client");
    file.close();                                       // Then close the file again
  }
  else {
    server.send(404, "text/html", "config file not found");
  }
}

void setupWebServer () {
  WebServerData.loadServerInfo();
  WebServerData.printStoredNetworks();

  server.on("/captive_portal", handleCaptivePortal);
  server.on("/scan_networks", sendScanNetworks);
  server.on("/wifisave", handleWifiSave);
  server.on("/server_info", handleServerInfo);
  server.on("/favicon.ico", [](){ server.send(404);});

  server.on("/config.json", handleConfigJson); // this is for development use only
  // reply to all requests with same HTML
  server.onNotFound([]() {
    // Serial.print("Handling not found:");
    // Serial.println(server.uri());
    handleCaptivePortal();
  });
  server.begin();
}