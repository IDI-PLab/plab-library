/*
 * LightFollower
 * 
 * Får Zumo robot til å følge etter en lyskilde.
 * 
 * Kretsen:
 *   1 x Zumo robot
 *   2 x lysavhengig motstand (LDR)
 *   2 x 10 k ohm motstand
 * Se her for mer info om Zumo robot:
 * https://www.ntnu.no/wiki/display/plab/6.+Pololu+Zumo+Robot
 * 
 * 
 * Make the Zumo robot follow a light source.
 * 
 * Circuit:
 *   1 x Zumo robot
 *   2 x light dependent resistor (LDR)
 *   2 x 10 k ohm resistor
 * Also see here for more information:
 * https://www.ntnu.no/wiki/display/plab/6.+Pololu+Zumo+Robot
 */
#include <ZumoMotors.h>
#include <Pushbutton.h>

const int MIN_SPEED = 0; // Minimum speed of our robot
const int MAX_SPEED = 200; // Maximum speed of our robot

const int LEFT_LIGHT_INPUT = A4; // Left light input pin
const int RIGHT_LIGHT_INPUT = A5; // Right light input pin

const int MIN_LIGHT = 800; // We assume some background light noise
const int MAX_LIGHT = 1024; // Maximum possible sensor reading

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);

// Helper function that reads light value and bounds it.
int readLightValue(int sensor) {
  int reading = analogRead(sensor);
  
  // Force bounds on reading, map will not do this automatically
  if (reading < MIN_LIGHT)
    reading = MIN_LIGHT;
  if (reading > MAX_LIGHT)
    reading = MAX_LIGHT;
  
  return reading;
}

void setup() {
  button.waitForButton();
}

void loop() {
  // Read light value
  int leftReading = readLightValue(LEFT_LIGHT_INPUT);
  int rightReading = readLightValue(RIGHT_LIGHT_INPUT);
  
  // Calculate motor speeds.
  // Notice left sensor control right motor and vice versa.
  int rightMotorSpeed = map(leftReading, MIN_LIGHT, MAX_LIGHT, MIN_SPEED, MAX_SPEED);
  int leftMotorSpeed = map(rightReading, MIN_LIGHT, MAX_LIGHT, MIN_SPEED, MAX_SPEED);
  
  // Start driving
  motors.setSpeeds(leftMotorSpeed, rightMotorSpeed);
  // Delay; no more than max 20 checks per second
  delay(50);
}
