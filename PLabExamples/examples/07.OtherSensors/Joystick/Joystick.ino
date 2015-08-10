/*
 * Joystick
 * 
 * Et eksempel som viser hvordan du kan lese tilstanden til en joystick
 * 
 * Kretsen:
 *   1 x joystick
 *   1 x 10 k ohm motstand.
 * Oppkobling kan finnes her:
 * https://www.ntnu.no/wiki/x/_BUPBQ
 * 
 * 
 * A simple example that demonstrates how to read joystick state.
 * 
 * Circuit:
 *   1 x joystick
 *   1 x 10 k ohm resistor
 * Also see:
 * https://www.ntnu.no/wiki/x/_BUPBQ
 */

// Pins used to read state
const int xAxisPin = A0;
const int yAxisPin = A1;
const int switchPin = 2;

// Initial position. Used as offset
int centerX = 0;
int centerY = 0;

void setup() {
  // Serial communicaion is the only debug info we will read
  Serial.begin(9600);

  // Set button pin as input
  pinMode(switchPin, INPUT);

  // Read center values for joystick
  centerX = analogRead(xAxisPin);
  centerY = analogRead(yAxisPin);
}

void loop() {
  // Update button and check state
  int buttonState = digitalRead(switchPin);
  if (buttonState == LOW) { // Pullup
    Serial.println("Button was pressed");
  } else {
    Serial.println("Button was NOT pressed");
  }

  // Read position of joystick
  int x = analogRead(xAxisPin);
  int y = analogRead(yAxisPin);

  Serial.print("Joystick RAW: (");
  Serial.print(x);
  Serial.print(" , ");
  Serial.print(y);
  Serial.println(")");

  // Use center postions as offset, and see relative positions
  x -= centerX;
  y -= centerY;
  Serial.print("Joystick offset: (");
  Serial.print(x);
  Serial.print(" , ");
  Serial.print(y);
  Serial.println(")");

  // Wait some time to not flood serial
  delay(500);
}
