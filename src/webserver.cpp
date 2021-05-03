#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include "wifiHelpers.h"

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
    Serial.println("file sent to client");
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
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
  Serial.println("wifi save");
  // char ssid[33] = "";
  // char password[65] = "";
  String ssid = server.arg("ssid");//.toCharArray(ssid, sizeof(ssid) - 1);
  String password = server.arg("pwd");//.toCharArray(password, sizeof(password) - 1);
  // there should be a check for if a password for the ssid is already stored
  connectWifi(ssid, password);
  Serial.print("ssid:\t"); Serial.println(ssid);
  Serial.print("password:\t"); Serial.println(password);
  server.sendHeader("Location", "wifi", true);
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.send(302, "text/plain", "");    // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.client().stop(); // Stop is needed because we sent no content length
}

void setupWebServer () {
  server.on("/captive_portal", handleCaptivePortal);
  server.on("/scan_networks", sendScanNetworks);
  server.on("/wifisave", handleWifiSave);
  // reply to all requests with same HTML
  server.onNotFound([]() {
    Serial.println("handling Not Found");
    String message = "URI: ";
    message += server.uri();
    Serial.println(message);
    handleCaptivePortal();
  });
  server.begin();
}