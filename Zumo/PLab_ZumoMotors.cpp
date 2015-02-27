#include "Arduino.h"
#include "PLab_ZumoMotors.h"
#include <ZumoMotors.h>


#define ADMUX_VCCWRT1V1 (_BV(MUX3) | _BV(MUX2) | _BV(MUX1))  // DS AREF..

float Read_Zumo_Volts_AREF()
{ 
  int oldADMUX = ADMUX;
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  if (ADMUX != ADMUX_VCCWRT1V1)
  {
    ADMUX = ADMUX_VCCWRT1V1;
    // Bandgap reference start-up time: max 70us
    // Wait for Vref to settle.
    delayMicroseconds(350); 
  }  
  // Start conversion and wait for it to finish.
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA,ADSC)) {};
    
  // Result is now stored in ADC.
  // Calculate Vcc (in V)
  float vcc = 1.1*1023.0 / ADC;

  // Apply compensation
  vcc = vcc * 1.77;  // Zumo divider factor;

  ADMUX = oldADMUX;
  return vcc; 
}

float calc_time(int _speed, float _length) {
  double a = 24069*pow(_speed,-1.238);
  double b = 5.7824*log(_speed) - 19.99;
  double _time = _length * a + b;
  float correction;
  if (_speed > 100) {
     correction = -0.0007*_speed + 1.154;
  } else if (_speed > 50) {
     correction = 0.0017*_speed + 0.9167;
  } else {
     correction = 0.0187*_speed + 0.0667;
  };
  _time = _time / correction;
  float battery_voltage = Read_Zumo_Volts_AREF();
  _time = _time * 5.2 / battery_voltage;
  return _time;
}

float calc_turn_length(int __speed) {
  float _speed = __speed;
  float y = 3.3375e-6*_speed*_speed*_speed - 0.00327*_speed*_speed + 1.0536*_speed - 28.595;
  float _length = 10.0 * 90.0 / y;
  return _length;
}

PLab_ZumoMotors::PLab_ZumoMotors()
{
}

void PLab_ZumoMotors::forward(int _speed, int _length)
{ 
  motors.setSpeeds(0, 0);
  float _time = calc_time(_speed,_length);
  motors.setSpeeds(_speed, _speed);
  delay(_time);
  motors.setSpeeds(0, 0);
}

void PLab_ZumoMotors::backward(int _speed, int _length)
{
  motors.setSpeeds(0, 0);
  float _time = calc_time(_speed,_length);
  motors.setSpeeds(- _speed, - _speed);
  delay(_time);
  motors.setSpeeds(0, 0);
}

void PLab_ZumoMotors::turnRight(int _speed, int degrees)
{  
  motors.setSpeeds(0, 0);
  float _length = calc_turn_length(_speed);
  _length = _length * degrees / 90.0;
  float _time = calc_time(_speed,_length);
  motors.setSpeeds(_speed, - _speed);
  delay(_time);
  motors.setSpeeds(0, 0);
}

void PLab_ZumoMotors::turnLeft(int _speed, int degrees)
{  
  motors.setSpeeds(0, 0);
  float _length = calc_turn_length(_speed);
  _length = _length * degrees / 90.0;
  float _time = calc_time(_speed,_length);
  motors.setSpeeds(- _speed, _speed);
  delay(_time);
  motors.setSpeeds(0, 0);
}

        // enable/disable flipping of motors
void PLab_ZumoMotors::flipLeftMotor(boolean flip) {
    motors.flipLeftMotor(flip);
}
    

void PLab_ZumoMotors::flipRightMotor(boolean flip) {
    motors.flipRightMotor(flip);
}
    
    // set speed for left, right, or both motors

void PLab_ZumoMotors::setLeftSpeed(int speed){
    motors.setLeftSpeed(speed);
}
    
void PLab_ZumoMotors::setRightSpeed(int speed){
    motors.setRightSpeed(speed);
}
    
void PLab_ZumoMotors::setSpeeds(int leftSpeed, int rightSpeed){
    motors.setSpeeds(leftSpeed, rightSpeed);
}