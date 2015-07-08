
/*
Drive randomly by changing motor speed
*/
#include <ZumoMotors.h>
#include <Pushbutton.h>


ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);

// This is the maximum speed the motors will be allowed to turn.
// (400 lets the motors go at top speed; decrease to impose a speed limit)
const int MAX_SPEED = 400;
int leftMotorSpeed = 100;
int rightMotorSpeed = 100;

void setup()
{
  randomSeed(analogRead(A0));  // Set seed for random psudo-number generation
  // Wait for the user button to be pressed and released
  button.waitForButton();
}

void loop()
{
   
  motors.setSpeeds(leftMotorSpeed, rightMotorSpeed); //set left and right motor speed
  delay(500);
  leftMotorSpeed = random(MAX_SPEED);
  rightMotorSpeed = random(MAX_SPEED);
}
