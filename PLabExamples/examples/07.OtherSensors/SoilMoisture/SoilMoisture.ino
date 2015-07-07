/*
  SoilMoisture

  Leser av analog og digital verdi for potteplantefuktighetssensor og skriver ut verdiene til seriell vindu.

  Kretsen:
    1 x fuktighetssensor
  Oppkobling vises på wiki:
  https://www.ntnu.no/wiki/pages/viewpage.action?pageId=80814688

  
  Reads a digital and an analog input from a soil moisture sensor and displays the result in console to the user.

  Circuit:
    1 x moisture sensor
  Connection is shown here:
  https://www.ntnu.no/wiki/pages/viewpage.action?pageId=80814688
  
  Created by Inge Edward Halsaunet, 2015
  Released into the public domain
*/

const int analogIn = A0; // The analog pin we will use
const int digitalIn = 2; // The digital pin we will use

void setup() {
  // Start communication wirh console
  Serial.begin(9600);
  // Set input mode for digial pin
  pinMode(digitalIn, INPUT);
}

void loop() {
  // Read digital input and tell result to user.
  Serial.print("Digital: ");
  if (digitalRead(digitalIn) == HIGH) {
    Serial.print("High\t");
  } else {
    Serial.print("Low\t");
  }
  
  // Read analog input and tell result to user.
  Serial.print("Analog: ");
  Serial.println(analogRead(analogIn));
  // Wait for 1 second before next read
  delay(1000);
}
