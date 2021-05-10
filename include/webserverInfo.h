#ifndef WEBSERVERINFO_H
#define WEBSERVERINFO_H
#include <WString.h>

class WebServerInfoClass
{
  private:
    String deviceName_;
    // std::map<String, String> SavedNetworks;

  public:
    WebServerInfoClass();

    static const size_t JsonResponseSize_ = 256; // TODO: calculate actual value instead of just guessing
    String currentNetwork;
    String savedSSIDS[10]; // max networks is set to 10, but multiple saved networks isn't supported yet

    // attribute readers
    String deviceName();
    int infoResponseSize();

    //functions
    void loadServerInfo();
    void getServerInfo(char* networkName);
};
#endif
