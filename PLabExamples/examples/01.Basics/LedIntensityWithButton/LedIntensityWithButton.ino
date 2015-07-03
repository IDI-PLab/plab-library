/*
 * LedIntensityWithButton
 * 
 * Slår av og på en lysdiode (LED) med en knapp, hvor er lysstyrken avhengig av et pot meter.
 * 
 * Kretsen:
 *   1 x LED
 *   1 x 220 ohm motstand
 *   1 x 10k ohm motstand
 *   1 x trykkknapp
 *   1 x pot (10k ohm eller 100k ohm)
 * Oppkoblingen kan sees her:
 * https://www.ntnu.no/wiki/display/plab/2.+Noen+enkle+eksempler+for+Arduino
 *   
 * 
 * Turns a LED on and off with a button, where intensity is dependent on a pot meter.
 * 
 * Circuit:
 *   1 x LED
 *   1 x 220 ohm resistor
 *   1 x 10k ohm resistor
 *   1 x push button
 *   1 x pot (10k ohm or 100k ohm)
 * How to hookup the circiut can be seen here:
 * https://www.ntnu.no/wiki/display/plab/2.+Noen+enkle+eksempler+for+Arduino
 */
const int ledPin = 6;
const int potPin = A0;
const int switchPin = 8;
int lightIntensity;
 
 
void setup(){
  pinMode( ledPin, OUTPUT ); 
  pinMode( switchPin, INPUT );
}
 
void loop(){
  if( digitalRead( switchPin ) == HIGH ){       // Her tester vi om bryteren er trykket inn
    lightIntensity = analogRead( potPin );      // Leser verdien fra potmeteret
    analogWrite( ledPin, lightIntensity / 4 );  // Setter lysstyrken på LED
  } else {
    analogWrite( ledPin, 0 ); // Setter lysstyrken til 0 hvis bryteren ikke er trykket inn 
  }
}
