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

saveNetwork(ssid, pwd) - adds ssid and pwd to storedNetworks, saves to local storage

changeDeviceName(String newName) - changes the name of the device in serverInfo, saves the name, and calls rebootRequired()

requireReboot() - sets rebootRequired to true
bool isRebootRequired() - returns value of rebootRequired

getStoredNetworks() - returns the list of saved ssids as an array (or iterator?)
getPassword(ssid) - returns the password of the ssid 
getDeviceName() - returns the device name
*/

#include "webserverInfo.h"
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <map>

const char *WebServerInfoClass::configFile = "config.json";

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
  StaticJsonDocument<JsonResponseSize_> info;
  info["deviceName"] = deviceName_;
  info["networkName"] = currentNetwork;
  serializeJson(info, bufferStr, JsonResponseSize_);
}

/*
 * Saves server info to local storage.
 * @return success
 */
bool WebServerInfoClass::saveServerInfo(){
  // saves the server info to local storage.
  StaticJsonDocument<JsonResponseSize_> info;
  info["deviceName"] = deviceName_;
  JsonObject storedNetworksJSON = info.createNestedObject("storedNetworks");
  // if (storedNetworksJSON == NULL){return false;}

  // I did try to keep this DRY, but couldn't manage it
  std::map<String, String>::iterator itr;
  for (itr = storedNetworks.begin(); itr != storedNetworks.end(); ++itr){
    storedNetworksJSON[itr->first] = itr->second;
  }
  // allStoredNetworks([storedNetworksJSON](String ssid, String pwd){storedNetworksJSON[ssid] = pwd;}); // capturing lambdas can't be used as function pointers. There are workarounds, but I don't understand them

  Serial.println("printing server info json");
  serializeJsonPretty(info, Serial);
  // File config = LittleFS.open
  return 1;
}

void WebServerInfoClass::loadServerInfo(){
  // loads the server info from local storage
  if(LittleFS.exists("serverInfo.json")){
    // actually load the server info
  }
  else{
    // create a new server info and put it in storage
    deviceName_ = "ESP Captive Portal";
    saveServerInfo();
  }
}

// -----------------------------------------------------------------------------------------------------
// -----------------------------------Update and Save the network---------------------------------------
// -----------------------------------------------------------------------------------------------------


/*
 * If ssid has a length, currentNetwork is updated and the new network is saved to local storage.
 * @return 0 if no network is passed, 1 if network is saved, -1 if storedNetworks is full
 */
int WebServerInfoClass::updateNetwork(String ssid, String pwd){
  currentNetwork = ssid;
  if(ssid.length() == 0) { return 0;}
  return saveNetwork(ssid, pwd);
}

/*
 * private function to save the networks to local storage
 * @return 1 if network is saved, 0 if not saved, -1 if storedNetworks is full
 */
int WebServerInfoClass::saveNetwork(String ssid, String pwd){
  // saves a new network
  addToStoredNetworks(ssid, pwd);
  // return 1;
  return saveServerInfo();
}

WebServerInfoClass WebServerData;

// -----------------------------------------------------------------------------------------------------
// -------------------------------storedNetworks quasi-dictionary functions------------------------------
// -----------------------------------------------------------------------------------------------------

/* Add to the list of networks, and increment N_networks if ssid is new */
void WebServerInfoClass::addToStoredNetworks(String ssid, String pwd){
  if( storedNetworks.count(ssid) == 0){ ++N_networks; }
  storedNetworks[ssid] = pwd;
}

/* Remove from the list of networks, and decrement N_networks if ssid existed */
void WebServerInfoClass::removeFromStoredNetworks(String ssid){
  if( storedNetworks.count(ssid) > 0){
    --N_networks;
    storedNetworks.erase(ssid);
  }
}

/*
 * Iterates over every network name in savedNetwork
 * @param callback lambda function or function pointer that takes String as a parameter
 */
void WebServerInfoClass::allStoredNetworkSSIDS(void (*callback)(String)){
  std::map<String, String>::iterator itr;
  for (itr = storedNetworks.begin(); itr != storedNetworks.end(); ++itr){
    callback(itr->first);
  }
  // allStoredNetworks([&, callback](String ssid, String pwd){callback(ssid);}); // would love to DRY this up, but it doesn't seem to like it
}

/*
 * Iterates over every network name and password in savedNetwork
 * @param callback lambda function or function pointer that takes two Strings as parameters
 */
void WebServerInfoClass::allStoredNetworks(void (*callback)(String ssid, String pwd)){
  std::map<String, String>::iterator itr;
  for (itr = storedNetworks.begin(); itr != storedNetworks.end(); ++itr){
    callback(itr->first, itr->second);
  }
}

/*
 * Prints the name of every network stored in storedNetworks
 */
void WebServerInfoClass::printStoredNetworks(){
  Serial.print("number of networks: "); Serial.println(NStoredNetworks());
  Serial.println("Saved wifi networks:");
  allStoredNetworkSSIDS([](String ssid){Serial.println(ssid);});
  Serial.println("...and that's everything");
}
