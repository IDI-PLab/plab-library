/*
PLCUPSetCodeAndURI

Demonstrates the most basic way of including a processing sketch and a processing sketch URI through PLCUP.

 */

#include <SoftwareSerial.h>
#include <PLabBTSerial.h>

#define rxPin 2   // Connect this to pin RXD on the BT unit.
#define txPin 3   // Connect this to pin TXD on the BT unit.

// Code actually contain vibrate example
// https://github.com/IDI-PLab/Examples/blob/master/ArduinoMobileIntegrationExamples/VibrateOnClick/VibrateOnClick.pde
const char code[] = "private PLabBridge plabBridge;"
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
    if (code[pos] == '\0') {
      // Rewind, so we can read the sketch multiple times
      pos = 0;
      return 0;
    }
    return 1;
  }
  char read() {
    return code[pos++];
  }
};


PLabBTSerial btSerial(txPin, rxPin);
CodeSource cSrc;  // It is IMPORTANT to set the code source to be accessible global. It will not be copied, and thus the space it occupy could be used for other variables.

void setup()
{
    Serial.begin(9600);

    // If both code and URI is set, they would prefferably contain the same code. But they need not do.
    btSerial.setPLCUPCodeSourcePointer(&cSrc);
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

