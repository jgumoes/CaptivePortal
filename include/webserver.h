#ifndef CONFIGPORTALWEBSERVER_H
  #include <ESP8266WebServer.h>
  #define CONFIGPORTALWEBSERVER_H
  #include <LittleFS.h>

  void setupWebServer();

  void handlePortalClient();

  bool handleCaptivePortal();
#endif