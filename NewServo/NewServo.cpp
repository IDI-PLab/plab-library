
#include "NewServo.h"

NewServo::NewServo(){
  servoPin = 6;
  cycleTime = 4800;
}

void NewServo::sendPWM(unsigned int microTime, int n, int servoPin) {
 // Send pulsetrain
  for (int i = 0; i<n; i++){
    digitalWrite(servoPin,HIGH);  // Set HIGH
    delayMicroseconds(microTime);
    digitalWrite(servoPin,LOW);
    delayMicroseconds(cycleTime-microTime); // set LOW rest of cycle
    //delayMicroseconds(cycleTime);
  }
}
void NewServo::attach(int pin){
  servoPin = pin;
  lastPosition = 0; // start position 0 degrees
  digitalWrite(servoPin, LOW);
  pinMode(servoPin, OUTPUT);
  
  // Put servo in start position
  // Use enough time: servo takes ~460 ms from 180 degrees to 0
  sendPWM( 550, 50, servoPin); 
}

void NewServo::write(int degrees){
  const int readyTime = 20000; // time to get servo "ready"
 // Convert  degrees to pulselength (see datasheet SG90: 500 and 2400)
 // but we use calibrated values found
  unsigned int microTime = map(degrees,0,180,550,2400); 
  
 // Get time to send pulses (until servo in desired position)
 // Datasheet: 100ms for 60 degrees (but needs 100 cycles 180 degrees gives 480 ms)
  float factor = cycleTime*100/180.; // no cycles pr. degrees
  float timeRotate = abs(lastPosition-degrees)*factor + readyTime;  //micro-sec
  
  // Map to number of pulses 
  // i.e number of cycles needed to rotate (must last rotation time)
  
  int n = timeRotate/cycleTime ;
  // Send pulsetrain and update last position
   sendPWM(microTime, n, servoPin);
   lastPosition = degrees;
}
