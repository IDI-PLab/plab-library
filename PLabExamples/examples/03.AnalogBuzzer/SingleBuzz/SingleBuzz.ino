/*
  SingleBuzz

  Spillet en enkelt tone på pinne 9 i en loop.

  Kretsen:
    1 x buzzer
    1 x 220 ohm resistor
  https://www.ntnu.no/wiki/display/plab/3.+Buzzere


  Plays a tone on output pin 9 in a loop.
  
  Circuit:
    1 x buzzer
    1 x 220 ohm resistor
  https://www.ntnu.no/wiki/display/plab/3.+Buzzere
   
  This example code is in the public domain.
*/
const int speakerOut = 9; // Put speaker through 220 ohm on pin 9.

const int frequency = 1760; // A6
const int toneDelay = 1000; // One second tone

void setup() {
  pinMode(speakerOut, OUTPUT);
}

void loop() {
  // Play the tone
  tone(speakerOut, frequency, toneDelay);
  // two delays: one for playing the actual tone and one for pause
  delay(toneDelay);
  delay(toneDelay);
}
