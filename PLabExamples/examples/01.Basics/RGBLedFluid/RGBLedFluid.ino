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

int r = 0;
int g = 0;
int b = 0;

void setup()
{
  // initialize the serial communications:
  pinMode(RGB_redpin, OUTPUT);   // sets the pin as output
  pinMode(RGB_greenpin, OUTPUT);   // sets the pin as output
  pinMode(RGB_bluepin, OUTPUT);   // sets the pin as output 
}

void loop()
{
  // Write the color values
  analogWrite(RGB_redpin, r); 
  analogWrite(RGB_greenpin, g); 
  analogWrite(RGB_bluepin, b); 
  // Make a delay, so we can see the colors change
  delay(100);
  // Increase the color values with different speeds
  r += 5;
  g += 7;
  b += 9;
  // Make sure we do not overflow maximum
  if (r > 255) {
    r -= 255;
  }
  if (g > 255) {
    g -= 255;
  }
  if (b > 255) {
    b -= 255;
  }
}
