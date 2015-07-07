/*
 * PIR
 * 
 * Leser av bevegelsessensor og tenner en LED når bevegelse er oppdaget.
 * 
 * Kretsen:
 *   1 x PIR sensor
 *   1 x LED
 *   1 x 220 ohm motstand
 * Oppkoblingen kan sees på wiki:
 * https://www.ntnu.no/wiki/display/plab/5.+Bevegelsessensor
 * 
 * 
 * Reads a movement sensor and lights a LED when movement has been detected.
 * 
 * Circuit:
 *   1 x PIR sensor
 *   1 x LED
 *   1 x 220 ohm resistor
 * How to connect circuit can be seen here:
 * https://www.ntnu.no/wiki/display/plab/5.+Bevegelsessensor
 */
boolean reading;

// Constants that define pins. Won't change
const int sensorPin = 5;
const int ledPin = 8;
 
void setup(){
  pinMode(sensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
}
 
void loop(){
  reading = digitalRead( sensorPin );
  if( reading == HIGH ){
    digitalWrite( ledPin, HIGH );
  } else {
    digitalWrite( ledPin, LOW );
  }
}
