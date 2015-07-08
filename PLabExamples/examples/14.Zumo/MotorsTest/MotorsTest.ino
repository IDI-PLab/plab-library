/*
 * MotorsTest
 * 
 * Makes the Zumo robot move in a triangular shape.
 */
#include <PLab_ZumoMotors.h>

// Example code for PLab Zumo libary:
// API:
// void forward(int _speed, int length);
// void backward(int _speed, int _length);
// void turnRight(int _speed, int degrees);
// void turnLeft(int _speed, int degrees); 
//
// Dag Svanæs 2015, IDI, NTNU


PLab_ZumoMotors PLab_motors;

void setup()
{
}

void loop()   // Draw a triangle. 45, 90, 45 degrees...
{
  int zumoSpeed = 200;  // Speed is 200 for all operations in this example

  delay(1000);
  PLab_motors.forward(zumoSpeed, 20);   // Forward 20 cm.
  delay(500);
  PLab_motors.turnRight(zumoSpeed,90);  // Turn right 90 degrees.
  delay(500);
  PLab_motors.forward(zumoSpeed, 20);   // Forward 20 cm.
  delay(500);
  PLab_motors.turnLeft(zumoSpeed,45);  // Turn left 45 degrees.
  delay(500);
  PLab_motors.backward(zumoSpeed, 28);   // Backward 28 cm.  
  delay(500);
  PLab_motors.turnLeft(zumoSpeed,45);  // Turn left 45 degrees.
  
}
