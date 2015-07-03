/*
  DigitalRead
  
  Reads a digital input on pin 2, prints the result to the serial monitor.
  Attach one side of a button to pin 2 and to a 10k ohm resistor. Attach the other side of the button to ground, and the other side of the resistor to +5V.
 
  This example code is in the public domain.
 */

const int inputPin = 2;      // Constant that define input pin

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = digitalRead(inputPin);
  // print out the value you read:
  Serial.println(sensorValue);
  delay(1);        // delay in between reads for stability
}
