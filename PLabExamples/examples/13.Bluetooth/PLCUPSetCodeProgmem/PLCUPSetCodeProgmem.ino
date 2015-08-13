/*
PLCUPSetCode

Demonstrates the most basic way of including a processing sketch through PLCUP.

 */

#include <SoftwareSerial.h>
#include <PLabBTSerial.h>

#include <avr/pgmspace.h> // Tools to read from program memory

#define rxPin 2   // Connect this to pin RXD on the BT unit.
#define txPin 3   // Connect this to pin TXD on the BT unit.


// Code actually contain vibrate example
// https://github.com/IDI-PLab/Examples/blob/master/ArduinoMobileIntegrationExamples/VibrateOnClick/VibrateOnClick.pde
// To save runtime memory, we place the code in program memory
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
"}";

class CodeSource : public AbstractPLabCodeSource {
private:
  int pos = 0;
public:
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
  char read() {
    // Read the character from program memory
    char c = pgm_read_byte_near(code + pos);
    // increment position
    pos++;
    // return the character
    return c;
  }
};


PLabBTSerial btSerial(txPin, rxPin);
CodeSource cSrc;

void setup()
{
    Serial.begin(9600);
    
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

