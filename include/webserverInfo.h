#ifndef WEBSERVERINFO_H
#define WEBSERVERINFO_H
#include <WString.h>
#include <map>

class WebServerInfoClass
{
  private:
    //variables
    String deviceName_;
    std::map<String, String> SavedNetworks;
    int N_SavedNetworks;

    // functions

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
};
  extern WebServerInfoClass WebServerData;
#endif
