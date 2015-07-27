/*
 * PushButtonToggle-v2
 * 
 * Slår på en LED når en knapp holdes inne. Knappen er "debounced".
 * Biblioteket som er inkludert gjør debouncingen.
 * 
 * Mer informasjon om debouncing kan finnes her:
 * https://www.arduino.cc/en/Tutorial/Debounce
 * 
 * Kretsen:
 *   1 x Trykknapp
 *   1 x 10k ohm motstand
 *   1 x 220 ohm motstand
 *   1 x LED
 * Oppkoblingen vises her:
 * https://www.ntnu.no/wiki/display/plab/1.+Generelt+om+sensorer+og+trykknapper
 *   
 * 
 * Turns a LED on while a button is pressed. The button is debounced.
 * The included library does the debouncing.
 * 
 * More information about debouncing can be found here:
 * https://www.arduino.cc/en/Tutorial/Debounce
 * 
 * Circuit:
 *   1 x push button
 *   1 x 10k ohm resistor
 *   1 x 220 ohm resistor
 *   1 x LED
 * How to connect circuit can be seen here:
 * https://www.ntnu.no/wiki/display/plab/1.+Generelt+om+sensorer+og+trykknapper
 */
 
#include <PLab_PushButton.h>

const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  3;      // the number of the LED pin
int ledState = LOW;          // State of LED.
PLab_PushButton myButton(buttonPin); // Create a PushButton object.

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);  
}

void loop() {
  myButton.update();  // Update: Read the switch.
  if (myButton.pressed()) {  // Was the button pressed?      
    if (ledState == LOW) {  // Toggle LED.
        ledState = HIGH; }
    else {
        ledState = LOW; };
  };
  digitalWrite(ledPin, ledState);
}
