/*
Terminal

This is a simple terminal for the BT module.

https://www.ntnu.no/wiki/pages/viewpage.action?pageId=78024129
 */

#include <SoftwareSerial.h>

#define rxPin 2   // Connect this to pin RXD on the BT unit.
#define txPin 3   // Connect this to pin TXD on the BT unit.


SoftwareSerial btSerial(txPin, rxPin);

void setup()
{
    Serial.begin(9600);
    btSerial.begin(9600);
}


void loop() {
    while (btSerial.available()) {
      char c = btSerial.read();
      Serial.write(c);
    };
    while (Serial.available()) {
      char c = Serial.read();
      btSerial.write(c);
    };    
}

