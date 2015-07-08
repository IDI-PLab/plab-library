
/*
Example of Zumo with LED,servo and ultrasonic sensor mounted in front.
Zumo goes forward in constant speed  until the sonar detects something within maxRange.
If within range: the LED is turned on, Zumo turns slighly away (left) from object detected
while turning servo forth and back (0-180) in steps
Pushbuttom must be pressed to start Zumo.    

    Sonar:  echoPin = 0
            triggerPin = 1
    LED:    ledPin = 2 
    Servo:  servoPin = 6
    
                        Asbjørn Thommasen - 2015
*/

#include <NewPing.h>
#include <NewServo.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>

#define FORWARD_SPEED 100 //Motor speed Zumo
#define SONAR_RANGE 50  // in cm
const int echoPin = 0; 
const int triggerPin = 1; 
// Max distance (in cm)
const int maxDistance = 20; // Limit for action 

const int ledPin=2;
const int servoPin = 6;
int degreesServo = 0;
int degreesStep = 30;


NewPing sonar(triggerPin, echoPin, SONAR_RANGE);  // make sonar and
NewServo myServo;                         // servo
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); 

void setup() {
   pinMode(ledPin,OUTPUT);
   myServo.attach(servoPin); 
   button.waitForButton(); // start when button pressed
   motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
}

void loop() {
  // Use sonar to detct objects within range and convert to distance
  unsigned int time = sonar.ping();
  float distance = sonar.convert_cm(time);
  //
  // check if inside range for action
  if (! distance > 0) { // sonar gives zero when outside range
    // Turn off LED and just go forward
    digitalWrite(ledPin,LOW); 
   motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
   } else {
   // Light LED and turn slighly away from object detected
   // while turning servo forth and back in steps (0-180)
     digitalWrite(ledPin,HIGH);
     myServo.write(degreesServo);
     degreesServo = degreesServo + degreesStep;
     if (degreesServo > 180) {
       degreesStep = -degreesStep;
       degreesServo = 180;
     } else if (degreesServo < 0) {
       degreesStep = -degreesStep;
       degreesServo = 0;
     }  
     // Turn way from object by rotating left (obstacle)
     motors.setSpeeds(-FORWARD_SPEED, FORWARD_SPEED);
   }
}
