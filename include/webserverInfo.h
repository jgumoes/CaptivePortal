#ifndef WEBSERVERINFO_H
#define WEBSERVERINFO_H
#include <WString.h>
#include <map>
#include <ArduinoJson.h>

#define NETWORK_STORAGE_FULL 100

/* Stores and processes the CaptivePortal state variables, including handling the config.
 * Please don't declare a new one of these, but you can use WebServerData, I made it especially for you
 */
class WebServerInfoClass
{
  private:
    //variables
    String deviceName_;
    static const char *configFileName_;

    // functions
    int saveNetwork(String ssid, String pwd);
    bool saveServerInfo();

    // storedNetworks variables
    std::map<String, String> storedNetworks;
    int8_t N_networks = 0;
    bool networkSaved = false;
    

    // storedNetworks functions
    void allStoredNetworks(void (*callback)(String, String));
    void addToStoredNetworks(String ssid, String pwd);
    void removeFromStoredNetworks(String ssid);
    void listStoredNetworks(const JsonArray& storedNetworksList);

  public:
    WebServerInfoClass();

    // variables
    static const size_t JsonResponseSize_ = 512; // TODO: calculate actual value instead of just guessing
    String currentNetwork;

    // attribute readers
    String deviceName();
    int infoResponseSize();
    bool isNetworkSaved();

    //functions
    bool loadServerInfo();
    String getServerInfo();

    bool updateNetwork(String ssid, String pwd);
    void removeNetworks(String ssid);

    // storedNetworks variables
    static const int8_t MaxStoredNetworks_ = 1;
    String configFileName(){ return String(configFileName_);};
    // storedNetworks functions
    int NStoredNetworks(){ return N_networks; }
    void allStoredNetworkSSIDS(void (*callback)(String));
    void printStoredNetworks();
    String storageFullResponseObj(String connectedNetwork);
};
  /* the only instance of WebServerInfoClass that should be allowed to exist */
  extern WebServerInfoClass WebServerData;

#endif