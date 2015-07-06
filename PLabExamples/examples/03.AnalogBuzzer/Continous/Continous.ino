/*
  Continous
  
  Reads an analog input on pin A0 and outputs a square
  wave with a frequency determined by the analog input.
  
  Attach the center pin of a potentiometer to pin A0, 
  and the outside pins to +5V and ground.
  The same code works for photoresistor setup on Pin A0.
  How to hook up the circuit can be seen at:
  https://www.ntnu.no/wiki/display/plab/3.+Buzzere
  
  Dag Svanæs, IDI, NTNU, 2015
   
  This example code is in the public domain.
*/
int speakerOut = 9; // Put speaker through 220 ohm on pin 9.
int frequency = 0;

void setup() {
  pinMode(speakerOut, OUTPUT);
}

// Function to produce one square pulse of a
// given frequency on a given pin:
void playOneSquarePulse(int pin, int freq) {
  long int pulseLength = 500000 / freq;
  digitalWrite(pin, HIGH);
  delayMicroseconds(pulseLength);
  digitalWrite(pin, LOW);
  delayMicroseconds(pulseLength);
}

void loop() {
  // read the input on analog pin 0: 
  int sensorValue = analogRead(A0);
  // Map sensor value 0..1023 to 100..3169 Hz.
  frequency = 100 + 3 * (sensorValue);
  playOneSquarePulse(speakerOut, frequency);
}
