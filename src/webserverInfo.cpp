/*
variables:
serverInfo (JSON) = 
  {
    "currentNetwork": null,
    "savedNetwork" :
      {
        "ssid": "pwd",
      },
    "deviceName": "CaptivePortal",
  }

bool rebootRequired = false; // this should idealy be clossed for modification

methods:

loadServerInfo() - loads serverInfo JSON from storage.
createServerInfo() - creates empty serverInfo json and stores it
saveServerInfo() - saves the serverInfo object to storage

saveNetwork(ssid, pwd) - adds ssid and pwd to savedNetworks, saves to local storage

changeDeviceName(String newName) - changes the name of the device in serverInfo, saves the name, and calls rebootRequired()

requireReboot() - sets rebootRequired to true
bool isRebootRequired() - returns value of rebootRequired

getSavedNetworks() - returns the list of saved ssids as an array (or iterator?)
getPassword(ssid) - returns the password of the ssid 
getDeviceName() - returns the device name
*/

#define _DICT_PACK_STRUCTURES
#define _DICT_KEYLEN 32

#include "webserverInfo.h"
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <map> // use this instead of Dictionary

// constructor
WebServerInfoClass::WebServerInfoClass(){
  currentNetwork = "";
  loadServerInfo();
}

// attribute readers
int WebServerInfoClass::infoResponseSize() { return JsonResponseSize_; }
String WebServerInfoClass::deviceName() { return deviceName_; }

//public functions
void WebServerInfoClass::getServerInfo(char* bufferStr){
  StaticJsonDocument<256> info;
  info["deviceName"] = deviceName_;
  info["networkName"] = currentNetwork;
  serializeJson(info, bufferStr, JsonResponseSize_);
}

// bool saveServerInfo(){
//   // saves the server info to local storage.
// }

// bool WebServerInfoClass::saveNetwork(String ssid, String pwd){
//   // saves a new network
// }

void WebServerInfoClass::loadServerInfo(){
  // loads the server info from local storage
  if(LittleFS.exists("serverInfo.json")){
    // actually load the server info
  }
  else{
    // create a new server info and put it in storage
    deviceName_ = "ESP Captive Portal";
    // saveServerInfo();
  }
}

WebServerInfoClass WebServerData;