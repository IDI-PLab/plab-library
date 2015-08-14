/*
PLCUPSetCodeAndURICallback

Demonstrates the most basic way of including a processing sketch and a processing sketch URI through PLCUP.
The example demonstrate the use of this with PLabBTCallback

 */

#include <SoftwareSerial.h>
#include <PLabBTCallback.h>

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
    if (code[pos] == '\0') {
      // Rewind, so we can read the sketch multiple times
      pos = 0;
      return 0;
    }
    return 1;
  }

  // Overriding AbstractPLabCodeSource read method
  char read() {
    return code[pos++];
  }
};

// Create the callback controller
// Last argument is message divider. The sign that is between the command and argument.
// We do not want to have any division in messages, so we set it to the same sign that divides messages: newline
PLabBTCallback btCallback(txPin, rxPin, '\n');
CodeSource cSrc;  // It is IMPORTANT to set the code source to be accessible global. It will not be copied, and thus the space it occupy could be used for other variables.

void setup()
{
    Serial.begin(9600);

    // If both code and URI is set, they would prefferably contain the same code. But they need not do.
    // Passing a pointer to the CodeSource object. Passing a pointer is the reason the CodeSource object should be global.
    btCallback.setPLCUPCodeSourcePointer(&cSrc);
    btCallback.setPLCUPCodeURI("https://raw.githubusercontent.com/IDI-PLab/Examples/master/ArduinoMobileIntegrationExamples/SimpleButtonExample/Processing/SimpleButtonExample/SimpleButtonExample.pde");

    // Register callback function. Only one function can be registered with this method.
    btCallback.registerNoCommandCallback(echoFunction);
    
    btCallback.begin(9600);
}


void loop() {
    btCallback.update();
    
    while (Serial.available()) {
      char c = Serial.read();
      btCallback.write(c);
    };    
}

// ---- The callback function ----
void echoFunction(char *arg) {
    // Print the incomming data to console
    Serial.print("Received data: ");
    Serial.println(arg);
}
