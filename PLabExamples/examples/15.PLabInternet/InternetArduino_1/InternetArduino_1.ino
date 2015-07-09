/* 
  PLab Simple Internet.
  The PLab_Internet library takes care of message passing in a circular 
  configuration of Arduinos using the serial port on pin 0 (Rx) and pin 1 (Tx).
  
  Connect the Arduinos with common ground.
  Connect Tx from Arduino to Rx on the next, and continue. 
  Connect Tx on the last to Rx on the first.
  
  With three Arduions with address 0, 1 and 2 it will look like:
  
  GND on Arduino 0 to GND on Arduino 1
  GND on Arduino 1 to GND on Arduino 2 
  
  Pin 1 (Tx) on Arduino 0  --> Pin 0 (Rx) on Arduino 1
  Pin 1 (Tx) on Arduino 1  --> Pin 0 (Rx) on Arduino 2
  Pin 1 (Tx) on Arduino 2  --> Pin 0 (Rx) on Arduino 3
  
  The PLab_Internet object has the following interface:
     PLab_Internet(byte my_address);   // Set address of this Arduino.
     void update();                    // update must be calles in loop.
     void sendMessage(byte receiver_address, String text);  // Send message
     void onReceive( void (*)(byte,String) );  // Define callback function
*/

#include "PLabInternet.h"

PLab_Internet mySocket(1);           // Socket object with ID=1.

void setup() {
  mySocket.onReceive(myMessageHandler); // Register event handler.
}

void myMessageHandler(byte senderID, String message) // Event handler
{
  // Serial.println(senderID); Serial.println(message);
}

void loop() {
  mySocket.update();  // Update: Read and write Serial port.
}
