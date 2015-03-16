#include <PLabInternet.h>

void (*PLab_Internet::user_onReceive)(byte,String);

PLab_Internet::PLab_Internet(byte my_address)
{
  _address = my_address;
  serialInitialized = false;
}

void PLab_Internet::onReceive( void (*function)(byte,String) )
{
  user_onReceive = function;
}

void PLab_Internet::update()
{ if (!serialInitialized) {
     Serial.begin(9600);
     serialInitialized = true;
  }

  byte receiverID = 0;
  byte senderID = 0; 
  String message = "";
  if (!user_onReceive) return;
  if ( Serial.available() == 0 ) return;
  int messageLength = Serial.read();
  while ( Serial.available() == 0 ) {};
  receiverID = Serial.read();  
  while ( Serial.available() == 0 ) {};
  senderID = Serial.read();  
  for (int i = 0; i < messageLength-2; i++) {
    while ( Serial.available() == 0 ) {};
    char c = Serial.read();
    message += String(c);
  };
  if (senderID == _address) return;  // It went all around..
  if (receiverID == _address) {  // It is for me
     user_onReceive(senderID,message); }
  else {                         // Forward it...
     byte len = message.length() + 2;
     Serial.write(len);
     Serial.write(receiverID);
     Serial.write(senderID);
     Serial.print(message);
  }
}


void PLab_Internet::sendMessage(byte receiver_address, String text)
{
  byte len = text.length() + 2;
  Serial.write(len);
  Serial.write(receiver_address);
  Serial.write(_address);
  Serial.print(text);
}

