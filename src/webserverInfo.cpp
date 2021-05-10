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



changeDeviceName(String newName) - changes the name of the device in serverInfo, saves the name, and calls rebootRequired()

requireReboot - sets rebootRequired to true
bool isRebootRequired() - returns value of rebootRequired
*/

#include <LittleFS.h>

bool saveServerInfo(){
  // saves the server info to local storage.
}

bool saveNetwork(String ssid, String pwd){
  // saves a new network
}

void loadServerInfo(){
  // loads the server info from local storage
  if(LittleFS.exists("serverInfo.json")){
    // actually load the server info
  }
  else{
    // create a new server info and put it in storage
    // saveServerInfo();
  }
}
