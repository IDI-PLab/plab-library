/*
 * PLabBTSerialEcho
 * 
 * A simple example that show how to do echo functionality using the PLabBTSerial library.
 * 
 * Circuit:
 *   1 x Bluetooth or Bluetooth LE module with UART interface
 * 
 * https://www.ntnu.no/wiki/pages/viewpage.action?pageId=78024129
 * 
 * Created by Inge Edward Halsaunet, 2015
 * Released into the public domain
 */

// Libraries:
// The SoftwareSerial is used for serial communcation using any two pins on the arduino.
// If PLabBTSerial is used, it must be included to make your code compile
#include <SoftwareSerial.h>
// PLabBTSerial is used to simplify gathering of text to lines. It extends SoftwareSerial.
#include <PLabBTSerial.h>

const int btUnitTxPin = 2; // Connected to tx on bt unit
const int btUnitRxPin = 3; // Connected to rx on bt unit
PLabBTSerial btSerial(btUnitTxPin, btUnitRxPin);

void setup() {
  // Start debug output
  Serial.begin(9600);
  Serial.println("Hello world!");
  // Start communication with your bluetooth device
  btSerial.begin(9600);
}

void loop() {
  // Check if some data has come from bt device
  int availableCount = btSerial.available();
  if (availableCount > 0) {
    // Create a char array (c-string) to hold the incomming data.
    char text[availableCount];
    // Read the incomming data. bufferSize refers to the buffer you are giving as an argument
    btSerial.read(text, availableCount);
    // Print the incomming data to console
    Serial.print("Received data: ");
    Serial.println(text);
    btSerial.println(text); // Echo text back via BT
  }
  
  // If some data has been written in the console, send this to bt unit
  while (Serial.available() > 0) {
    char letter = Serial.read();
    btSerial.write(letter);
  }
}
