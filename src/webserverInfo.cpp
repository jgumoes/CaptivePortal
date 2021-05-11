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

#include "webserverInfo.h"
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <map>

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

// -----------------------------------------------------------------------------------------------------
// -----------------------------------Update and Save the network---------------------------------------
// -----------------------------------------------------------------------------------------------------


/*
 * If ssid has a length, currentNetwork is updated and the new network is saved to local storage.
 * @return 0 if no network is passed, 1 if network is saved, -1 if savedNetworks is full
 */
int WebServerInfoClass::updateNetwork(String ssid, String pwd){
  currentNetwork = ssid;
  if(ssid.length() == 0) { return 0;}
  return saveNetwork(ssid, pwd);
}

/*
 * private function to save the networks to local storage
 * @return 1 if network is saved, -1 if savedNetworks is full
 */
int WebServerInfoClass::saveNetwork(String ssid, String pwd){
  // saves a new network
  addToSavedNetworks(ssid, pwd);
  return 1;
  // return saveServerInfo()
}

WebServerInfoClass WebServerData;

// -----------------------------------------------------------------------------------------------------
// -------------------------------savedNetworks quasi-dictionary functions------------------------------
// -----------------------------------------------------------------------------------------------------

/* Add to the list of networks, and increment N_networks if ssid is new */
void WebServerInfoClass::addToSavedNetworks(String ssid, String pwd){
  if( savedNetworks.count(ssid) == 0){ ++N_networks; }
  savedNetworks[ssid] = pwd;
}

/* Remove from the list of networks, and decrement N_networks if ssid existed */
void WebServerInfoClass::removeFromSavedNetworks(String ssid){
  if( savedNetworks.count(ssid) > 0){
    --N_networks;
    savedNetworks.erase(ssid);
  }
}

/*
 * Iterates over every network in savedNetwork
 * @param callback lambda function or function pointer that takes String as a parameter
 */
void WebServerInfoClass::allSavedNetworkSSIDS(void (*callback)(String)){
  std::map<String, String>::iterator itr;
    for (itr = savedNetworks.begin(); itr != savedNetworks.end(); ++itr){
      callback(itr->first);
    }
}

/*
 * Prints the name of every network stored in savedNetworks
 */
void WebServerInfoClass::printSavedNetworks(){
  Serial.print("number of networks: "); Serial.println(NSavedNetworks());
  Serial.println("Saved wifi networks");
  allSavedNetworkSSIDS([](String ssid){Serial.println(ssid);});
  Serial.println("...and that's everything");
}
