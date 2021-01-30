#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
//#include <ESP8266mDNS.h>
#include <EEPROM.h>

/*
   This example serves a "hello world" on a WLAN and a SoftAP at the same time.
   The SoftAP allow you to configure WLAN parameters at run time. They are not setup in the sketch but saved on EEPROM.

   Connect your computer or cell phone to wifi network ESP_ap with password 12345678. A popup may appear and it allow you to go to WLAN config. If it does not then navigate to http://192.168.4.1/wifi and config it there.
   Then wait for the module to connect to your wifi and take note of the WLAN IP it got. Then you can disconnect from ESP_ap and return to your regular WLAN.

   Now the ESP8266 is in your network. You can reach it through http://192.168.x.x/ (the IP you took note of) or maybe at http://esp8266.local too.

   This is a captive portal because through the softAP it will redirect any http request to http://192.168.4.1/
*/

unsigned long portal_timeout = 60000;  // portal timeout in mS. default should be 60 seconds
unsigned long portal_state = 1;       // indicates if the portal should be opened or closed
unsigned long portal_time;            // the portal's expiry time, in milliseconds
int p_time = millis();  // for testing purposes. async delay on print commands

boolean wrong_pass = false;
boolean network_lost = false;
uint8_t n_networks = 0;

/* Set these to your desired softAP credentials. They are not configurable at runtime */
#ifndef APSSID
#define APSSID "ESP_ap"
#define APPSK  "12345678"
#endif

const char *softAP_ssid = APSSID;
const char *softAP_password = APPSK;

/* hostname for DNS in softAP mode. Try http://esp8266.local */
const char *myHostname = "esp8266";

/* Don't set this wifi credentials. They are configurated at runtime and stored on EEPROM */
char ssid[33] = "";
char password[65] = "";

// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

// Web server
ESP8266WebServer server(80);

/* Soft AP network parameters */
IPAddress apIP(172, 217, 28, 1);
IPAddress netMsk(255, 255, 255, 0);


/** Should I connect to WLAN asap? */
boolean connect;

/** Last time I tried to connect to WLAN */
unsigned long lastConnectTry = 0;

/** Current WLAN status */
unsigned int status = WL_IDLE_STATUS;

int connectWifi() {
  Serial.println("Connecting as wifi client...");
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  int connRes = WiFi.waitForConnectResult();
  Serial.print("connRes: ");
  Serial.println(connRes);
  return connRes;
}

int connectWifi(char* Ssid, char* Password) {
  Serial.println("Connecting as wifi client...");
  WiFi.disconnect();
  WiFi.begin(Ssid, Password);
  int connRes = WiFi.waitForConnectResult();
  Serial.print("connRes: ");
  Serial.println(connRes);
  return connRes;
}

void setup() {
  //deleteCredentials();  for testing only!!!
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP(softAP_ssid);//, softAP_password);
  delay(500); // Without delay I've seen the IP address blank
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  /*char dns_url[strlen(softAP_ssid) + 6];
  strcpy(dns_url, softAP_ssid);
  strcat(dns_url, ".local");*/
  dnsServer.start(DNS_PORT, "*", apIP);   // this needs to be "*" to capture sign in requests

  /* Setup web pages: root, wifi config pages, SO captive portal detectors and not found. */
  server.on("/", handleRoot);
  server.on("/wifisave", handleWifiSave);
  server.on("/advanced_settings", handleWifiSave);
  server.on("/generate_204", handleRoot);  //Android captive portal. Maybe not needed. Might be handled by notFound handler.
  server.on("/fwlink", handleRoot);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  server.onNotFound(handleNotFound);
  server.begin(); // Web server start
  Serial.println("HTTP server started");
  loadCredentials(); // Load WLAN credentials from network
  if(strlen(ssid) > 0){ // Request WLAN connect if there is a SSID
    connectWifi();
  }
  else{
    Serial.println("no SSID found");
  }
  
  status = connectWifi();

  Serial.printf("WiFi connection %s \n", status?"succesfull":"unsuccesfull");

  unsigned long reconnect_delay = 10000;
  unsigned long no_conn_millis = millis() + reconnect_delay;
  while(status != 3){
    // while not connected, hold portal open
    // reattempt wifi connection after 10 seconds
    //Serial.print(no_conn_millis); Serial.print("\t"); Serial.println(millis());
    if(no_conn_millis < millis()){
      //Serial.print("Wifi not connected : \t");
      loadCredentials();
      if(strlen(ssid) > 0){ // Request WLAN connect if there is a SSID
      //Serial.print("attempting to reconnect");
      status = connectWifi();
      }
      else{
        Serial.println("no SSID found");
      }
      no_conn_millis = millis() + reconnect_delay;
    }
    //DNS
    dnsServer.processNextRequest();
    //HTTP
    server.handleClient();
    delay(50);
  }
  Serial.println("exciting setup");
  portal_time = millis() + portal_timeout;

  p_time = millis();  // for testing purposes. async delay on print commands
  delay(500);
  Serial.print("p_time = "); Serial.print("\t millis = "); Serial.println(millis());
}



void loop() {
  /*
  if (p_time < millis()){
    Serial.print(portal_time);
    Serial.print("\t");
    Serial.println( millis());
    p_time += 1000;
  }
  */
  if(portal_state == 1){
      
    if(portal_time > millis()){// > portal_timeout){
    // if the portal hasn't timed out or portal hasn't been closed yet
      //Serial.println(portal_time);
      //Serial.println( millis());
      if(WiFi.softAPgetStationNum() > 0){
        // if there are devices connected, extend the portal 
        portal_time = millis() + portal_timeout;  // RIP Serial Monitor
        //Serial.println("Devices detected; portal extended");
      }
      else{
        Serial.println(WiFi.softAPgetStationNum());
      }
      Serial.print("\n");
    }
    else{
      // close the portal
      WiFi.softAPdisconnect(true);
      Serial.println("portal closed");
      portal_state = 0;
    }
  }
  // Do work:
  //DNS
  dnsServer.processNextRequest();
  //HTTP
  server.handleClient();
  delay(50);
  
}
