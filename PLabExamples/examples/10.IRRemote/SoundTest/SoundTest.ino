
/*
 * SoundTest
 * 
 * Mottar signal fra en fjernkontroll og spiller A4 eller A5 hvis knappen var 1 eller 2.
 * 
 * Kretsen:
 *   1 x IR fjernkontrollkit
 *   2 x LED
 *   3 x 220 ohm motstand
 *   1 x buzzer
 * Mer informasjon om tilkobling kan leses her:
 * https://www.ntnu.no/wiki/display/plab/15.+IR+fjernkontroll
 * 
 * 
 * Receives a signal from a remote and plays A4 or A5 if the button pushed was 1 or 2.
 * 
 * Circuit:
 *   1 x IR remote kit
 *   2 x LED
 *   3 x 220 ohm resistor
 *   1 x buzzer
 * More information about connecting the circuit can be seen here:
 * https://www.ntnu.no/wiki/display/plab/15.+IR+fjernkontroll
 */
#include "PLab_IRremote.h"

/*-----( Declare Constants )-----*/
int const receiverPin = 11; // pin Y of IR receiver to Arduino digital pin 11
int const redLEDPin = 2;
int const greenLEDPin = 3;
int const speakerOut = 9; // Put speaker through 220 ohm on pin 9.

/*-----( Declare objects )-----*/
IRrecv irrecv(receiverPin);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  irrecv.enableIRIn(); // Start the receiver
  irrecv.blink13(false); // Do not blink pin 13 as feedback.
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(speakerOut, OUTPUT);
}

// Function to produce one square pulse of a
// given frequency on a given pin:
void playTone(int pin, int freq, int dur) {
  long int pulseLength = 500000 / freq;
  long int durLong = dur;
  long int noOfPulses = (durLong*1000) / (pulseLength*2);
  for (long int i = 0; i < noOfPulses; i++) {
     digitalWrite(pin, HIGH);
     delayMicroseconds(pulseLength);
     digitalWrite(pin, LOW);
     delayMicroseconds(pulseLength);
  }
}
void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  if (irrecv.decode(&results)) // have we received an IR signal?
  { switch(results.value) {
      case IR_1: 
         digitalWrite(redLEDPin, HIGH); 
         playTone(speakerOut, 440, 500);
         digitalWrite(redLEDPin, LOW);          
       break;
       case IR_2: 
         digitalWrite(greenLEDPin, HIGH); 
         playTone(speakerOut, 880, 500);
         digitalWrite(greenLEDPin, LOW);          
       break;
    }// End switch
    irrecv.resume(); // receive the next value
  }  
}/* --(end main loop )-- */

