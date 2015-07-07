/*
 * RGBLedFluid
 * 
 * Endrer lyset til en RGB LED gradvis mens den blander fargene.
 * 
 * Kretsen:
 *   1 x RGB LED
 *   3 x 220 ohm motstand
 * Oppkoblingen vises på wiki:
 * https://www.ntnu.no/wiki/display/plab/1.+Generelt+om+aktuatorer+og+LED
 * 
 * 
 * Changes the color of a RGB LED while mixing the colors.
 * 
 * Circuit:
 *   1 x RGB LED
 *   3 x 220 ohm resistor
 * How to connect circuit can be seen here:
 * https://www.ntnu.no/wiki/display/plab/1.+Generelt+om+aktuatorer+og+LED
 */
// these constants describe the pins. Since we use analogWrite, they must be pwm pins. They won't change:
const int RGB_redpin = 9;
const int RGB_greenpin = 10;
const int RGB_bluepin = 11;

void setup()
{
  // initialize the serial communications:
  pinMode(RGB_redpin, OUTPUT);   // sets the pin as output
  pinMode(RGB_greenpin, OUTPUT);   // sets the pin as output
  pinMode(RGB_bluepin, OUTPUT);   // sets the pin as output 
}

void loop()
{
  // print the sensor values:
  for (int r=0; r <= 1; r++) {
    for (int g=0; g <= 1; g++) {
      for (int b=0; b <= 1; b++) {
         analogWrite(RGB_redpin, r*255); 
         analogWrite(RGB_greenpin,g*255); 
         analogWrite(RGB_bluepin,b*255); 
         delay(500);
  }}}
}
