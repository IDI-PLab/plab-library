/*
Prints raw reflectance-sensor data in serial window
*/
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>

Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
 
ZumoReflectanceSensorArray reflectanceSensors; 
 
void setup()
{
   Serial.begin(9600);
   reflectanceSensors.init();
   button.waitForButton();
}

void loop()
{
  reflectanceSensors.read(sensor_values); // gives raw values 0-2000
  Serial.print(sensor_values[0]);
  Serial.print(' ');
  Serial.print(sensor_values[5]);
  Serial.println(' ');
  delay(500);  
}
