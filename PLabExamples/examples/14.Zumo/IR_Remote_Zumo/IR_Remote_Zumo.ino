
#include "PLab_IRremote.h"
#include <ZumoMotors.h>
#include <Pushbutton.h>

/*-----( Declare Constants )-----*/
const int receiverPin = 2; // pin Y of IR receiver to Arduino digital pin 11
const int SPEED = 200;

/*-----( Declare objects )-----*/
IRrecv irrecv(receiverPin);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);


void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  irrecv.enableIRIn(); // Start the receiver
  button.waitForButton();
}

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  if (irrecv.decode(&results)) // have we received an IR signal?
  { switch(results.value) {
      case IR_UP: motors.setSpeeds(SPEED, SPEED); break;
      case IR_DOWN: motors.setSpeeds(-SPEED, -SPEED); break;
      case IR_RIGHT: motors.setSpeeds(SPEED, -SPEED); break;
      case IR_LEFT: motors.setSpeeds(-SPEED, SPEED); break;
      case IR_OK: motors.setSpeeds(0, 0); break;
    }// End switch
    irrecv.resume(); // receive the next value
  }  
}/* --(end main loop )-- */


