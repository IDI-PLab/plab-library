/*
  Touch

  Dette er et eksempel som viser hvordan en touchknapp kan slå av og på et lys.

  Kretsen:
    1 x touchknapp
    1 x 220 ohm motstand
    1 x LED
  Oppkobling vises på wiki:
  https://www.ntnu.no/wiki/display/plab/11.+Touch+sensor

  
  This is an example that show how a light can be turned on and off by a touch button.
  
  Circuit
    1 x touch button
    1 x 220 ohm resistor
    1 x LED
  How to connect circuit can be seen here:
  https://www.ntnu.no/wiki/display/plab/11.+Touch+sensor
  
  Created by Inge Edward Halsaunet, 2015
  Released into the public domain
*/


const int touchIn = 2;    // Digital pin the touch button is connected to
const int ledOut = 3;   // Digital pin the led is connected to

int touchValue = 0;  // Will store our touch reading

void setup() {
  // Initialize inputs and outputs
  pinMode(touchIn, INPUT);
  pinMode(ledOut, OUTPUT);
}

void loop() {
  // Read the current touch button state
  touchValue = digitalRead(touchIn);
  
  // Update output light value
  digitalWrite(ledOut, touchValue);
  // Short delay to ensure sensor stability
  delay(1);
}
