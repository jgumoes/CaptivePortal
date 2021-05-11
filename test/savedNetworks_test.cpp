#include <unity.h>
// #include <WString.h>
#include "../include/webserverInfo.h"

int main( int argc, char **argv) {
  UNITY_BEGIN();
  TEST_ASSERT_EQUAL_INT(0, WebServerData.NSavedNetworks());
  UNITY_END();
}