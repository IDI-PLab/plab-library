/*
  Tests the use of two sonars (ultrasound sensors)
  Reads distance every second, and 
  prints distance in serial window

  Circuit
    2 x HC-SR04 (ultrasonic sensor)
  How to connect circuit can be seen here:
  https://www.ntnu.no/wiki/display/plab/3.+Ultralyd+-+HC-SR04
 
      Asbjørn Thomassen - 2015
*/
#include <NewPing.h>
// The pins we will receive echo calls from
const int echoPin1 = 2;
const int echoPin2 = 4;
// The pins we will use to send the signal
const int triggerPin1 = 3;
const int triggerPin2 = 5;
// Max distance (in cm)
const int maxDistance = 30;

// Initialize two sonars with trigger pin, echo pin and max distance
NewPing sonar1(triggerPin1, echoPin1, maxDistance);
NewPing sonar2(triggerPin2, echoPin2, maxDistance);

void setup() {
  // Starting serial communication to print to pc
  Serial.begin(9600);
}

void loop() {
  // Get the time sonar 1 and 2
  unsigned int time1 = sonar1.ping();
  unsigned int time2 = sonar2.ping();
  // Find distance
  float distance1 = sonar1.convert_cm(time1);
  float distance2 = sonar2.convert_cm(time2); 
  // Print 
  Serial.println(distance1);
  Serial.print(" ");
  Serial.println(distance2);
  // And wait for a second before we repeat
  delay(1000); 
}
