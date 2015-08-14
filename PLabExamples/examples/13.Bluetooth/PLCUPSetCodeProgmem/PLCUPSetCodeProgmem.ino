/*
PLCUPSetCodeProgmem

Demonstrates the most basic way of including a processing sketch through PLCUP.
The Processing code is stored in program memory to save runtime memory. The example demonstrate how this can be done.

 */

#include <SoftwareSerial.h>
#include <PLabBTSerial.h>

#include <avr/pgmspace.h> // Tools to read from program memory

#define rxPin 2   // Connect this to pin RXD on the BT unit.
#define txPin 3   // Connect this to pin TXD on the BT unit.


// Code actually contain vibrate example
// https://github.com/IDI-PLab/Examples/blob/master/ArduinoMobileIntegrationExamples/VibrateOnClick/VibrateOnClick.pde
// To save runtime memory, we place the code in program memory, indicated by the variable modifier PROGMEM
const char code[] PROGMEM = "private PLabBridge plabBridge;"
"void bindPLabBridge (PLabBridge bridge) {"
  "plabBridge = bridge;"
  "size (bridge.getWidth (), bridge.getHeight ());"
"}"

"boolean previousPressed = false;"

"void setup() {"
  "size(240, 360);"
"}"

"void draw() {"
  "if (mousePressed != previousPressed) {"
    "previousPressed = mousePressed;"
    "if (mousePressed) {"
      "if (plabBridge != null) {"
        "plabBridge.vibrate(1000);"
      "}"
    "}"
  "}"
  "if (mousePressed) {"
    "background(#FF0033);"
  "} else {"
    "background(#333333);"
  "}"
"}"; // Notice the semicolon is only here, after the entire code.


// CodeSource is a class that feed PLabBT*** with Processing code that should be delivered to the app.
// It inherits from AbstracPLabCodeSource, an abstract base class that provides the methods that are needed to feed code.
class CodeSource : public AbstractPLabCodeSource {
// private: This area is not visible outside the class.
private:
  int pos = 0;
  
// public: This area is accessible for everyone.
public:
  // Overriding AbstractPLabCodeSource available method
  int available() {
    // Code is in program memory, so we need to fetch the following character to see if it is the end of the string
    char c = pgm_read_byte_near(code + pos);
    if (c == '\0') {
      // Rewind, so we can read the sketch multiple times
      pos = 0;
      return 0;
    }
    return 1;
  }
  
  // Overriding AbstractPLabCodeSource read method
  char read() {
    // Read the character from program memory
    char c = pgm_read_byte_near(code + pos);
    // increment position
    pos++;
    // return the character
    return c;
  }
};


PLabBTSerial btSerial(txPin, rxPin);  // Serial connection to remote device
CodeSource cSrc;    // Instance of CodeSource. THIS SHOULD BE A GLOBAL VARIABLE!

void setup()
{
    Serial.begin(9600);
    
    // Passing a pointer to the CodeSource object. Passing a pointer is the reason the CodeSource object should be global.
    btSerial.setPLCUPCodeSourcePointer(&cSrc);
    
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

