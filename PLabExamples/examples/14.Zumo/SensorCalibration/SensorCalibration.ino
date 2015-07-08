/*
Calibrates reflectancesensor in setup by manually moving Zumo
over actual areas for 10 sec.
Then prints calibrated sensor data in serial window
*/
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>

ZumoReflectanceSensorArray reflectanceSensors;

// Define an array for holding sensor values.
#define NUM_SENSORS 6
unsigned int sensorValues[NUM_SENSORS];

void setup()
{
  Serial.begin(9600);
  reflectanceSensors.init();

  delay(500);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);        // turn on LED to indicate we are in calibration mode
  
  unsigned long startTime = millis();
  while(millis() - startTime < 10000)   // make the calibration take 10 seconds
  {
    reflectanceSensors.calibrate();
  }
  digitalWrite(13, LOW);         // turn off LED to indicate we are through with calibration
}

void loop()
{
  // Get calibrated sensor data
  unsigned int position = reflectanceSensors.readLine(sensorValues); 
  
  for (byte i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print(' ');
  }
  Serial.print("    ");
  Serial.println(position);
  
  delay(250);
}
