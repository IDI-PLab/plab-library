/*
  PLab_ZumoMotors.h - Library for Zumo.
  Created by Dag Svanæs, 2015.
  Released into the public domain.
*/

#ifndef PLab_ZumoMotors_h
#define PLab_ZumoMotors_h

#include "Arduino.h"
#include <ZumoMotors.h>

class PLab_ZumoMotors
{
  public:
    PLab_ZumoMotors();
    void forward(int _speed, int length);
    void backward(int _speed, int _length);
    void turnRight(int _speed, int degrees);
    void turnLeft(int _speed, int degrees);   
    
        // enable/disable flipping of motors
    void flipLeftMotor(boolean flip);
    void flipRightMotor(boolean flip);
    
    // set speed for left, right, or both motors
    void setLeftSpeed(int speed);
    void setRightSpeed(int speed);
    void setSpeeds(int leftSpeed, int rightSpeed);
    
  private:
    ZumoMotors motors;
};

#endif