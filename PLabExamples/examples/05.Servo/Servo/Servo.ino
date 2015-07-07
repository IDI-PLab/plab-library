/*
 * Servo
 * 
 * Bruker det innebygde biblioteket Servo for å kontrollere en servo.
 * Eksempelet flytter servoen basert på avlest verdi fra et potentiometer.
 * 
 * Kretsen:
 *   1 x servo
 *   1 x 10/100 k ohm potentiometer
 * Oppkobling vises på wiki:
 * https://www.ntnu.no/wiki/display/plab/2.+Servoer
 * 
 * 
 * This example uses the NewServo library to control a servo.
 * The example moves the servo based on read value from a potentiometer.
 * 
 * Curcuit
 *   1 x servo
 *   1 x 10/100 k ohm potentiometer
 * How to connect circuit can be seen here:
 * https://www.ntnu.no/wiki/display/plab/2.+Servoer
 */
#include <Servo.h>    // Inkluderer Servo-iblioteket i koden
 
 
Servo minServo;       // Lager et servo-objekt
 
 
const int potPin = A5;
const int servoPin = 9;
int potVerdi;
int vinkel;
 
 
void setup(){
  minServo.attach(servoPin); // Knytter servo-objektet til pinne 9
}
 
void loop(){
  potVerdi = analogRead(potPin);
  vinkel = map(potVerdi, 0, 1023, 0, 179);
  minServo.write(vinkel);
  delay(15);
}
