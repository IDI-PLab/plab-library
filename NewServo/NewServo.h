/** Arduino Uno ServoPWM Servo library **

This library for the Arduino Uno uses direct PWM
to control a single servo.  This can be useful for
people who cannot use the Arduino IDE's Servo library with a sonar.  For
example, the ZumoMotors library uses the same timer as the Servo
library (Timer 1), so the two libraries conflict. 
The sonar uses Timer2 with similar problems

  Asbjørn Thomassen
*/

// ensure this library description is only included once
#ifndef NewServo_h
#define NewServo_h

#include "Arduino.h"
#include <inttypes.h>


class NewServo
{
 public:
     // constructor:
    NewServo();
	void attach(int pin);
    void write(int degrees);
    void sendPWM(unsigned int microTime, int n, int servoPin);
 private:
    int lastPosition; // in degrees
    int servoPin; // pin connected to servo
    long int cycleTime; // ms
};

#endif