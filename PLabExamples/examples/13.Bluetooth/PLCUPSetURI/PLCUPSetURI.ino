/*
PLCUPSetURI

Demonstrates the most basic way of including a processing sketch URI through PLCUP.

https://www.ntnu.no/wiki/pages/viewpage.action?pageId=78024129
 */

#include <SoftwareSerial.h>
#include <PLabBTSerial.h>

#define rxPin 2   // Connect this to pin RXD on the BT unit.
#define txPin 3   // Connect this to pin TXD on the BT unit.


PLabBTSerial btSerial(txPin, rxPin);  // Serial connection to remote device

void setup()
{
    Serial.begin(9600);
    
    btSerial.setPLCUPCodeURI("https://raw.githubusercontent.com/IDI-PLab/Examples/master/ArduinoMobileIntegrationExamples/SimpleButtonExample/Processing/SimpleButtonExample/SimpleButtonExample.pde");
    
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

