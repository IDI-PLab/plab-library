/*
   Hello world
   
   Opens serial communication, and sends "Hello world" every second.
   PLab example, IDI, NTNU, 2015.
*/

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  Serial.println("Hello world");
  delay(1000);        // One second delay.
}
