/*
 ButtonTwoBlinks
 
 Blinks a LED connected to digital pin 13 twice when a pushbutton
 attached to pin 2 is pressed
 
 
 The circuit:
 * LED attached from pin 13 to ground 
 * pushbutton attached to pin 2 from ground
 * Pullup 10K resistor attached to pin 2 from +5V
 
 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.
 How to connect circuit can be seen here:
 https://www.ntnu.no/wiki/display/plab/1.+Generelt+om+sensorer+og+trykknapper
 
Dag Svanæs
IDI, NTNU, 2015.

*/
// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);     
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is LOW:
  if (buttonState == LOW) {       // Blink LED:                                    
    digitalWrite(ledPin, HIGH); 
    delay(500);
    digitalWrite(ledPin, LOW); 
    delay(500);
    digitalWrite(ledPin, HIGH); 
    delay(500);
    digitalWrite(ledPin, LOW);  
  }
}
