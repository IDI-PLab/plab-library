/*
 * PushButtonTwoToggles-v2
 * 
 * Slår på en LED når en knapp holdes inne.
 * To knapper og to LED er tilgjengelig.
 * Knappene er "debounced".
 * Biblioteket som er inkludert gjør debouncingen.
 * 
 * Mer informasjon om debouncing kan finnes her:
 * https://www.arduino.cc/en/Tutorial/Debounce
 * 
 * Kretsen:
 *   2 x Trykknapp
 *   2 x 10k ohm motstand
 *   2 x 220 ohm motstand
 *   2 x LED
 * Oppkoblingen vises her:
 * https://www.ntnu.no/wiki/display/plab/1.+Generelt+om+sensorer+og+trykknapper
 *   
 * 
 * Turns a LED on while a button is pressed.
 * Two buttons and two LEDs are available.
 * The buttons are debounced.
 * The included library does the debouncing.
 * 
 * More information about debouncing can be found here:
 * https://www.arduino.cc/en/Tutorial/Debounce
 * 
 * Circuit:
 *   2 x push button
 *   2 x 10k ohm resistor
 *   2 x 220 ohm resistor
 *   2 x LED
 * How to connect circuit can be seen here:
 * https://www.ntnu.no/wiki/display/plab/1.+Generelt+om+sensorer+og+trykknapper
 */
#include <PLab_PushButton.h>

const int redButtonPin = 2;     // the number of the pushbutton pin
const int redLedPin =  3;      // the number of the LED pin
int redLedState = LOW;          // State of LED.
PLab_PushButton redButton(redButtonPin); // Create a PushButton object.

const int greenButtonPin = 4;     // the number of the pushbutton pin
const int greenLedPin =  5;      // the number of the LED pin
int greenLedState = LOW;          // State of LED.
PLab_PushButton greenButton(greenButtonPin); // Create a PushButton object.

void setup() {
  // initialize the LED pin as an output:
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);  
}

void loop() {
  redButton.update();  // Update: Read the switch.
  greenButton.update();  // Update: Read the switch.
  
  if (redButton.pressed()) {  // Was the button pressed?      
    if (redLedState == LOW) {  // Toggle LED.
        redLedState = HIGH; }
    else {
        redLedState = LOW; };
  };
  digitalWrite(redLedPin, redLedState);
  
  if (greenButton.pressed()) {  // Was the button pressed?      
    if (greenLedState == LOW) {  // Toggle LED.
        greenLedState = HIGH; }
    else {
        greenLedState = LOW; };
  };
  digitalWrite(greenLedPin, greenLedState);   
}
