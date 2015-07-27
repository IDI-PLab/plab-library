/*
 * PressedEvent
 * 
 * Ser når en knapp blir trykket på, og sender seriell beskjed når det skjer.
 * 
 * Kretsen:
 *   1 x Trykknapp
 *   1 x 10k ohm motstand
 * Oppkoblingen vises her:
 * https://www.ntnu.no/wiki/display/plab/1.+Generelt+om+sensorer+og+trykknapper
 *   
 * 
 * Sees when a button is pushed, and sends a serial message when this occurs.
 * 
 * Circuit:
 *   1 x push button
 *   1 x 10k ohm resistor
 * How to connect circuit can be seen here:
 * https://www.ntnu.no/wiki/display/plab/1.+Generelt+om+sensorer+og+trykknapper
 */
 
// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin

// variables will change:
int buttonState;         // variable for reading the pushbutton status
int prevoiusButtonState = HIGH; // State of button at previous read. 

void setup() {    
   // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);     
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  // check if the pushbutton is being pressed.
  // if it is, the buttonState is LOW and was HIGH:
  if ((buttonState == LOW) && (prevoiusButtonState == HIGH)) {         
       Serial.println("Button pressed.");
  };
  prevoiusButtonState = buttonState;
}
