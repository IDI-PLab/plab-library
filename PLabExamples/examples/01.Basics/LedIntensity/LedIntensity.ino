/*
 * LedIntensity
 * 
 * Slår på en lysdiode (LED) med lysstyrke avhengig av et pot meter.
 * 
 * Kretsen:
 *   1 x LED
 *   1 x 220 ohm motstand
 *   1 x pot (10k ohm eller 100k ohm)
 * Oppkoblingen kan sees her:
 * https://www.ntnu.no/wiki/display/plab/2.+Noen+enkle+eksempler+for+Arduino
 *   
 * 
 * Turns a LED on with intensity dependent on a pot meter.
 * 
 * Circuit:
 *   1 x LED
 *   1 x 220 ohm resistor
 *   1 x pot (10k ohm or 100k ohm)
 * How to hookup the circiut can be seen here:
 * https://www.ntnu.no/wiki/display/plab/2.+Noen+enkle+eksempler+for+Arduino
 */

const int ledPin = 6;
const int potPin = A0;
int lightIntensity;
 
 
void setup(){
  pinMode( ledPin, OUTPUT ); 
}

void loop(){
  lightIntensity = analogRead( potPin );
  analogWrite( ledPin, lightIntensity / 4 );  // Deler lightIntensity på 4 for å få en verdi mellom 0 og 255
}
