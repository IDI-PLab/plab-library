/*
 * PLabBTCallbackEcho
 * 
 * A simple example that show how to do echo functionality using the PLabBTCallback library.
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
// If PLabBTCallbakck is used, it must be included to make your code compile
#include <SoftwareSerial.h>
// PLabBTCallback is used get callback functionality from Bluetooth. It uses PLabBTSerial, which extends SoftwareSerial.
#include <PLabBTCallback.h>

const int btUnitTxPin = 2; // Connected to tx on bt unit
const int btUnitRxPin = 3; // Connected to rx on bt unit

// Create the callback controller
// Last argument is message divider. The sign that is between the command and argument.
// We do not want to have any division in messages, so we set it to the same sign that divides messages: newline
PLabBTCallback btCallback(btUnitTxPin, btUnitRxPin, '\n');


void setup()
{
  // Start debug output
  Serial.begin(9600);
  Serial.println("Hello world!");
  
  // Register callback function. Only one function can be registered with this method.
  btCallback.registerNoCommandCallback(echoFunction);
  
  // Start communication with Bluetooth device
  btCallback.begin (9600);
}

void loop()
{
  // Get the callback controller to update its data structures.
  btCallback.update();
  
  // If some data has been written in the console, send this to bt unit
  while (Serial.available() > 0) {
    char letter = Serial.read();
    btCallback.write(letter);
  }
}

// ---- The callback function ----
void echoFunction(char *arg) {
  // Print the incomming data to console
  Serial.print("Received data: ");
  Serial.println(arg);
  btCallback.println(arg); // Echo text bac via BT
}

