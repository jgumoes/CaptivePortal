#ifndef WEBSERVERINFO_H
#define WEBSERVERINFO_H
#include <WString.h>
#include <map>

/* Stores and processes the CaptivePortal state variables, including handling the config.
 * Please don't declare a new one of these, but you can use WebServerData, I made it especially for you
 */
class WebServerInfoClass
{
  private:
    //variables
    String deviceName_;

    // functions
    int saveNetwork(String ssid, String pwd);

    // savedNetworks variables
    std::map<String, String> savedNetworks;
    int8_t N_networks = 0;

    // savedNetworks functions
    void addToSavedNetworks(String ssid, String pwd);
    void removeFromSavedNetworks(String ssid);

  public:
    WebServerInfoClass();

    // variables
    static const size_t JsonResponseSize_ = 256; // TODO: calculate actual value instead of just guessing
    String currentNetwork;
    // String savedSSIDS[10]; // replace with pointers? max networks is set to 10, but multiple saved networks isn't supported yet

    // attribute readers
    String deviceName();
    int infoResponseSize();

    //functions
    void loadServerInfo();
    void getServerInfo(char* networkName);

    int updateNetwork(String ssid, String pwd);

    // savedNetworks variables
    static const int8_t MaxSavedNetworks_ = 5;
    // savedNetworks functions
    int NSavedNetworks(){ return N_networks; }
    void allSavedNetworkSSIDS(void (*callback)(String));
    void printSavedNetworks();
};
  /* the only instance of WebServerInfoClass that should be allowed to exist */
  extern WebServerInfoClass WebServerData;

#endif