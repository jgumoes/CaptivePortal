#include <ESP8266WebServer.h>
#include <LittleFS.h>

ESP8266WebServer webServer(80);

void handlePortalClient(){
  webServer.handleClient();
}

bool handleCaptivePortal() { // send the right file to the client (if it exists)
  const char* path = "/config.html";
  String contentType = "text/html";            // Get the MIME type
  if (LittleFS.exists(path)) {                            // If the file exists
    File file = LittleFS.open(path, "r");                 // Open it
    webServer.streamFile(file, contentType); // And send it to the client
    Serial.println("file sent to client");
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}

void setupWebServer () {
  webServer.on("/captive_portal", handleCaptivePortal);
  // replie to all requests with same HTML
  webServer.onNotFound([]() {
    Serial.println("handling Not Found");
    String message = "URI: ";
    message += webServer.uri();
    Serial.println(message);
    handleCaptivePortal();
  });
  webServer.begin();
}