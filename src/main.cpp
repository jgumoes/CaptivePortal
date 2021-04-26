#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(172, 217, 28, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

#include <LittleFS.h>
// const char* fsName = "LittleFS";
// FS* fileSystem = &LittleFS;
// LittleFSConfig fileSystemConfig = LittleFSConfig();

String responseHTML = ""
                      "<!DOCTYPE html><html lang='en'><head>"
                      "<meta name='viewport' content='width=device-width'>"
                      "<title>CaptivePortal</title></head><body>"
                      "<h1>Hello World!</h1><p>This is a captive portal example."
                      " All requests will be redirected here.</p></body></html>";

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

void listDir(const char * dirname) {
  Serial.printf("Listing directory: %s\n", dirname);

  Dir root = LittleFS.openDir(dirname);

  while (root.next()) {
    File file = root.openFile("r");
    Serial.print("  FILE: ");
    Serial.print(root.fileName());
    Serial.print("  SIZE: ");
    Serial.print(file.size());
    file.close();
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("sketch started");
  WiFi.mode(WIFI_AP);
  Serial.println("WIFI started");
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("CaptivePortal");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  Serial.println("Mount LittleFS");
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }

  listDir("/");

  webServer.on("/captive_portal", handleCaptivePortal);
  // replay to all requests with same HTML
  webServer.onNotFound([]() {
    Serial.println("handling Not Found");
    String message = "URI: ";
    message += webServer.uri();
    Serial.println(message);
    handleCaptivePortal();
    // webServer.send(200, "text/plain", "");
  });
  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}