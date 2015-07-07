/*
  Microphone

  Dette er et eksempel som viser hvordan en mikrofon kan slå av og på et lys.

  Kretsen:
    1 x elektret mikrofon
    1 x 220 ohm motstand
    1 x LED
  Oppkobling vises på wiki:
  https://www.ntnu.no/wiki/display/plab/10.+Mikrofon

  
  This is an example that show how a light can be turned on and off by a high sound.
  
  Circuit
    1 x Electret microphone
    1 x 220 ohm resistor
    1 x LED
  How to connect circuit can be seen here:
  https://www.ntnu.no/wiki/display/plab/10.+Mikrofon
  
  Created by Inge Edward Halsaunet, 2015
  Released into the public domain
*/


const int micIn = 2;    // Digital pin the microphone is connected to
const int ledOut = 3;   // Digital pin the led is connected to

int micValue = 0;  // Will store our microphone reading

void setup() {
  // Initialize inputs and outputs
  pinMode(micIn, INPUT);
  pinMode(ledOut, OUTPUT);
}

void loop() {
  // Read the current microphone level
  micValue = digitalRead(micIn);
  
  // Update output light value
  digitalWrite(ledOut, micValue);
  // Short delay to ensure sensor stability
  delay(1);
}
