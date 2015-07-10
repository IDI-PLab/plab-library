/*
 * LightDependentResistor
 * 
 * Dette eksempelet leser en lysavhengig motstand og printer verdi hvert 0,3 sekund.
 * 
 * Kretsen:
 *   1 x LDR
 *   1 x 10 k ohm motstand
 * https://www.ntnu.no/wiki/display/plab/2.+Lyssensor
 * 
 * 
 * This example reads a light dependent resistor and prints the value to serial out every 0.3 second.
 * 
 * Circuit:
 *   1 x LDR
 *   1 x 10 k ohm resistor
 * https://www.ntnu.no/wiki/display/plab/2.+Lyssensor
 */
void setup(){
  Serial.begin(9600);
}
  
void loop(){
  Serial.println(analogRead(A0));
  delay(300);
}
