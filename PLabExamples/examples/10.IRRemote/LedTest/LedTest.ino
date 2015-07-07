/*
 * LedTest
 * 
 * Mottar signal fra en fjernkontroll og styrer lys om det var en piltast.
 * 
 * Kretsen:
 *   1 x IR fjernkontrollkit
 *   2 x LED
 *   2 x 220 ohm motstand
 * Mer informasjon om tilkobling kan leses her:
 * https://www.ntnu.no/wiki/display/plab/15.+IR+fjernkontroll
 * 
 * 
 * Receives a signal from a remote and controls lights if it were arrow keys.
 * 
 * Circuit:
 *   1 x IR remote kit
 *   2 x LED
 *   2 x 220 ohm resistor
 * More information about connecting the circuit can be seen here:
 * https://www.ntnu.no/wiki/display/plab/15.+IR+fjernkontroll
 */
#include "PLab_IRremote.h"

/*-----( Declare Constants )-----*/
int const receiverPin = 11; // pin Y of IR receiver to Arduino digital pin 11
int const redLEDPin = 2;
int const greenLEDPin = 3;

/*-----( Declare objects )-----*/
IRrecv irrecv(receiverPin);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  irrecv.enableIRIn(); // Start the receiver
  irrecv.blink13(false); // Do not blink pin 13 as feedback.
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
}

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  if (irrecv.decode(&results)) // have we received an IR signal?
  { switch(results.value) {
      case IR_UP: digitalWrite(redLEDPin, HIGH); break;
      case IR_DOWN: digitalWrite(redLEDPin, LOW); break;
      case IR_RIGHT: digitalWrite(greenLEDPin, HIGH); break;
      case IR_LEFT: digitalWrite(greenLEDPin, LOW); break;
    }// End switch
    irrecv.resume(); // receive the next value
  }  
}/* --(end main loop )-- */

