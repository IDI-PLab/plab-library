#ifndef PLAB_BT_INTERNET
#define PLAB_BT_INTERNET

#include <Arduino.h>

class PLab_Internet
{
public:
  PLab_Internet(byte my_address);
  void update();
  void sendMessage(byte receiver_address, String text);
  void onReceive( void (*)(byte,String) );

private:
  byte _address;
  boolean serialInitialized;
  static void (*user_onReceive)(byte,String);
};

#endif