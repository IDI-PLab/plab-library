/*
 * CodeDump
 * 
 * Mottar signal fra en fjernkontroll og skriver ut hvilken knapp som ble trykket.
 * 
 * Kretsen:
 *   1 x IR fjernkontrollkit
 * Mer informasjon om tilkobling kan leses her:
 * https://www.ntnu.no/wiki/display/plab/15.+IR+fjernkontroll
 * 
 * 
 * Receives a signal from a remote and prints which button was pushed.
 * 
 * Circuit:
 *   1 x IR remote kit
 * More information about connecting the circuit can be seen here:
 * https://www.ntnu.no/wiki/display/plab/15.+IR+fjernkontroll
 */
#include "PLab_IRremote.h"

/*-----( Declare Constants )-----*/
int receiver = 11; // pin 1 of IR receiver to Arduino digital pin 11

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'
/*-----( Declare Variables )-----*/

void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);
  Serial.println("Test:");
  irrecv.enableIRIn(); // Start the receiver
  irrecv.blink13(false); // DO not blink pin 13 as feedback.
}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
//    Serial.println(results.value, HEX);  // UN Comment to see raw values
    printIRcode(results.value); 
    irrecv.resume(); // receive the next value
  }  
}/* --(end main loop )-- */

/*-----( Declare User-written Functions )-----*/
void printIRcode(int value) // takes action based on IR code received
{
  switch(value)
  {
  case IR_UP: Serial.println(" UP"); break;
  case IR_LEFT: Serial.println(" LEFT");    break;
  case IR_OK: Serial.println(" -OK-");    break;
  case IR_RIGHT: Serial.println(" RIGHT");   break;
  case IR_DOWN: Serial.println(" DOWN"); break;
  case IR_1: Serial.println(" 1");    break;
  case IR_2: Serial.println(" 2");    break;
  case IR_3: Serial.println(" 3");    break;
  case IR_4: Serial.println(" 4");    break;
  case IR_5: Serial.println(" 5");    break;
  case IR_6: Serial.println(" 6");    break;
  case IR_7: Serial.println(" 7");    break;
  case IR_8: Serial.println(" 8");    break;
  case IR_9: Serial.println(" 9");    break;
  case IR_STAR: Serial.println(" *");    break;
  case IR_0: Serial.println(" 0");    break;
  case IR_HASH: Serial.println(" #");    break;
  case IR_REPEAT: Serial.println(" REPEAT");break;  
  default: 
    Serial.println(" other button");
  }// End Case
} //END translateIR


