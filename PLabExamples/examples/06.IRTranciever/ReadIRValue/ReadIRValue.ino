/*
  ReadIRValue
  
  Dette eksempelet leser inn en analog verdi hvert 0,5 sekund og skriver ut avlesningen.
  Det er ment som å vise hvordan du kan bruke IR sensoren TCRT5000, og viser hvordan du kan skrive ut verdier til skjermen.
  
  Kretsen
    1 x TCRT5000 (IR sender/mottaker)
    1 x 220 ohm motstand
    1 x 10 k ohm motstand
  Oppkoblingen vises på wiki:
  https://www.ntnu.no/wiki/display/plab/4.+IR+-+TCRT5000

  
  
  This example reads an analogue value each 0.5 seconds and prints the read value.
  It is intended to show how you can use the IR sensor TCRT5000, and shows how you can print the read value to screen.
  
  Circuit
    1 x TCRT5000 (IR tranciever)
    1 x 220 ohm resistor
    1 x 10 k ohm resistor
  How to connect curcuit is shown here:
  https://www.ntnu.no/wiki/display/plab/4.+IR+-+TCRT5000
*/

// Bestemmer hvilken analog input som skal brukes
// Decides which analogue input to use
const int sensorPin = A0;

void setup() {
  // Start seriekommunikasjon med konsollen
  // Start serial communication with console
  Serial.begin(9600);
}

void loop() {
  // Les verdien som kommer fra sensoren. Verdien er mellom 0 og 1024
  // Read the value coming from the sensor. The value is between 0 and 1024
  int value = analogRead(sensorPin);
  // Skriver ut resultatet
  // Prints the result
  Serial.println(value);
  // Vent ett halvt sekund før vi går videre
  // Wait half a second before proceeding.
  delay(500);
}
