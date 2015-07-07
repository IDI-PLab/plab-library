/*
 * NewServo
 * 
 * Bruker biblioteket NewServo for å kontrollere en servo.
 * Eksempelet flytter servoen til 0 grader, 90 grader, 180 grader og gjentar.
 * 
 * Kretsen:
 *   1 x servo
 * Oppkobling vises på wiki:
 * https://www.ntnu.no/wiki/display/plab/2.+Servoer
 * 
 * Bibliotek:
 *   - NewServo ( https://github.com/IDI-PLab/plab-library/tree/master/NewServo )
 * 
 * 
 * This example uses the NewServo library to control a servo.
 * The example sets the servo to 0 degrees, 90 degrees, 180 degrees and repeats.
 * 
 * Curcuit
 *   1 x servo
 * How to connect circuit can be seen here:
 * https://www.ntnu.no/wiki/display/plab/2.+Servoer
 * 
 * Library:
 *   - NewServo ( https://github.com/IDI-PLab/plab-library/tree/master/NewServo )
 */

#include <NewServo.h>

#define SERVO_PIN 0
NewServo servo;


void setup()
{
  // initialise servo
  servo.attach(SERVO_PIN);
}
 
void loop()
{
  // Turn servo 0, 90 and 180 degrees - and repeat
  servo.write(0);
  delay(1000);
  servo.write(90);
  delay(1000);
  servo.write(180);
  delay(1000);
}
