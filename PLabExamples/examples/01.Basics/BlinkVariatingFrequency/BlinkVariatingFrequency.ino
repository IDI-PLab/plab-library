/*
 * BlinkVariatingFrequency
 * 
 * Slår på en lysdiode (LED) en liten stund, før den slåes like lenge.
 * 
 * Kretsen:
 *   1 x LED
 *   1 x 220 ohm motstand
 *   1 x pot (10k ohm eller 100k ohm)
 * Oppkoblingen kan sees her:
 * https://www.ntnu.no/wiki/display/plab/2.+Noen+enkle+eksempler+for+Arduino
 *   
 * 
 * Turns a LED on for a little while, before it is turned off for the same length of time.
 * 
 * Circuit:
 *   1 x LED
 *   1 x 220 ohm resistor
 *   1 x pot (10k ohm or 100k ohm)
 * How to hookup the circiut can be seen here:
 * https://www.ntnu.no/wiki/display/plab/2.+Noen+enkle+eksempler+for+Arduino
 */
const int ledPin = 7;   // LED kobles til pinne 7
const int potPin = A0;  // Potmeter kobles til analog inngang 0 (A0)
int delayTime;          // Variabelen delayTime av type int holder verdien vi leser av på A0
 
void setup(){
  pinMode( ledPin, OUTPUT );        // Setter pinne 7 til utgang
}
 
void loop(){
  delayTime = analogRead( potPin ); // Leser av A0 og lagrer verdien i variabelen delayTime
  digitalWrite( ledPin, HIGH );     // Slår på LED
  delay(delayTime);                 // Venter i delayTime antall millisekunder
  digitalWrite( ledPin, LOW );      // Slår av LED
  delay(delayTime);                 // Venter i delayTime antall millisekunder
}
