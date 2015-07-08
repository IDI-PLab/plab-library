/*
ButtonTest

This is a simple terminal for the BT module.

https://www.ntnu.no/wiki/pages/viewpage.action?pageId=78024129
 */

#include <SoftwareSerial.h>
#include <PLabBTSerial.h>
#include <PLab_PushButton.h>

#define rxPin 2   // Connect this to pin RXD on the BT unit.
#define txPin 3   // Connect this to pin TXD on the BT unit.
const int buttonPin = 6;     // the number of the pushbutton pin
const int ledPin = 7;     // the number of the pushbutton pin

PLab_PushButton myButton(buttonPin); // Create a PushButton object.
PLabBTSerial btSerial(txPin, rxPin);

void setup()
{
   btSerial.begin(9600);
   pinMode(ledPin, OUTPUT);
}

void loop() {
   myButton.update();  // Update: Read the button.
   if (myButton.pressed()) {  // Was the button pressed?  
      digitalWrite(ledPin, HIGH);  
      btSerial.write("STEPF\r\n");  // All commands should end with \r\n  

   };  
   if (myButton.released()) {  // Was the button pressed?  
      digitalWrite(ledPin, LOW); 
      btSerial.write("STEPB\r\n");  // All commands should end with \r\n   
 
   };     
}

