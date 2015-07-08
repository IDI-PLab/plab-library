/*
Controls Zumo with LED forward/backwards with ultrasonic sensor mounted in front.
Measured distance is mapped to motorspeed, and Zumo goes forward in constant speed 
until the sonar detects something within maxRange.
If within range: the LED is turned on, and dependent of distance Zumo stops, and, 
if much closer than maxRange, Zumo moves backwards.
Pushbuttom must be pressed to start Zumo.    

    Sonar:  echoPin = 0
            triggerPin = 1
    LED:    ledPin = 2 
    
                        Asbjørn Thommasen - 2015
*/
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <NewPing.h>

#define FORWARD_SPEED   200
#define MAX_DISTANCE    25  // in cm (limit for action)
// --Setup for ultrasound--
// The pin we will receive echo calls from
const int echoPin = 0;  // Must be a PWM pin
// The pin we will use to send the signal
const int triggerPin = 1;
 
const int ledPin = 2;
// Initialize a "sonar" with trigger pin, echo pin and max distance

NewPing sonar(triggerPin, echoPin, MAX_DISTANCE);
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); 

void setup() {
   pinMode(ledPin,OUTPUT); 
   button.waitForButton();
}

void loop() {
  // Get the time for one ping trip
  unsigned int time = sonar.ping();
  // Calculate how far this time represent
  float distance = sonar.convert_cm(time);
 if (distance > 0) {
   digitalWrite(ledPin,HIGH);
 } else {
   digitalWrite(ledPin,LOW);
   distance = MAX_DISTANCE; // Forward full speed if no object
 } 
  // Map the 5 to 25 distance range into speed forward and backwards
  // ( 5 cm distance means full reverese speed)
 int dSpeed = map(distance, 5, MAX_DISTANCE, -FORWARD_SPEED, FORWARD_SPEED);    
  
  motors.setSpeeds(dSpeed, dSpeed);
}
 
