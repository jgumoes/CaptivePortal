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

const char *WebServerInfoClass::configFileName_ = "config.json";

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
  Serial.println("Saving Server Info");
  StaticJsonDocument<JsonResponseSize_> info;
  info["deviceName"] = deviceName_;
  JsonObject storedNetworksJSON = info.createNestedObject("storedNetworks");
  if (storedNetworksJSON.isNull()){return false;}
  Serial.print("Max number of networks"); Serial.println(N_networks);
  if(N_networks > MaxStoredNetworks_){ Serial.println("max stored networks reached"); return 0; }

  // I did try to keep this DRY, but couldn't manage it
  std::map<String, String>::iterator itr;
  for (itr = storedNetworks.begin(); itr != storedNetworks.end(); ++itr){
    storedNetworksJSON[itr->first] = itr->second;
  }
  // allStoredNetworks([storedNetworksJSON](String ssid, String pwd){storedNetworksJSON[ssid] = pwd;}); // capturing lambdas can't be used as function pointers. There are workarounds, but I don't understand them

  File configFile = LittleFS.open(configFileName_, "w");
  if (!configFile){ return false; }
  // configFile.write();
  serializeJson(info, configFile);
  return true;
}

/*
 * Loads the server info from the local storage. If none exists, it creates one.
 * @returns true if no errors were encountered
 */
bool WebServerInfoClass::loadServerInfo(){
  Serial.println("loadServerInfo");
  if(LittleFS.exists(configFileName_)){
    // actually load the server info
    StaticJsonDocument<JsonResponseSize_> info;
    File configFile = LittleFS.open(configFileName_, "r");
    DeserializationError error = deserializeJson(info, configFile);
    if(error){ return false; }

    deviceName_ = info["deviceName"] | "ESP Captive Portal";  // load the deviceName

    // load the saved networks
    JsonObject storedNetworksJSON = info["storedNetworks"];
    for (JsonPair network : storedNetworksJSON ) {
      addToStoredNetworks(network.key().c_str(), network.value());
    }
    printStoredNetworks();
    return true;
  }
  else{
    // create a new server info and put it in storage
    deviceName_ = "ESP Captive Portal";
    return saveServerInfo();
  }
}

// -----------------------------------------------------------------------------------------------------
// -----------------------------------Update and Save the network---------------------------------------
// -----------------------------------------------------------------------------------------------------


/*
 * If ssid has a length, currentNetwork is updated and the new network is saved to local storage.
 * @return 1 if network is saved, 0 if storedNetworks is full, -1 if no network is passed
 */
int WebServerInfoClass::updateNetwork(String ssid, String pwd){
  currentNetwork = ssid;
  if(ssid.length() == 0) { return -1;}
  return saveNetwork(ssid, pwd);
}

/*
 * private function to save the networks to local storage
 * @return 1 if network is saved, 0 if storedNetworks is full, -1 if not saved
 */
int WebServerInfoClass::saveNetwork(String ssid, String pwd){
  // saves a new network
  addToStoredNetworks(ssid, pwd);
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

/* Prepares a response object containing a list of stored networks
 * todo: refactor this whole process to pass a reference to the server
 * @returns the storage ojbect in full as a string.
 */
String WebServerInfoClass::storageFullResponseObj(){
  StaticJsonDocument<JsonResponseSize_> responseInfo;
  responseInfo["error"] = "storageFull";
  JsonArray storedNetworksJSON = responseInfo.createNestedArray("storedNetworks");
  // I did try to keep this DRY, but couldn't manage it
  std::map<String, String>::iterator itr;
  for (itr = storedNetworks.begin(); itr != storedNetworks.end(); ++itr){
    storedNetworksJSON.add(itr->first);
  }
  String responseObj;
  serializeJson(responseInfo, responseObj);
  return responseObj;
}
