/*
Demonstrates one second simple drive forward in setup
*/

#include <ZumoMotors.h>
#include <Pushbutton.h>


ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);


// This is the maximum speed the motors will be allowed to turn.
// (400 lets the motors go at top speed; decrease to impose a speed limit)
const int MAX_SPEED = 400;


void setup()
{
  // Wait for the user button to be pressed and released
  button.waitForButton();
  
  motors.setSpeeds(100,100); //set left and right motor speed
  delay(1000);
  motors.setSpeeds(0,0);
}

void loop()
{
  
}
