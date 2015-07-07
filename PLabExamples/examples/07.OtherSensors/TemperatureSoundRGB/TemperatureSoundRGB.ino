/*
 * TemperatureSoundRGB
 * 
 * Leser av temperaturen ved hjelp av analog temperaturssensor LM35.
 * Varsler med lyssignal om temperaturen er over eller under 25 grader.
 * Lager lyd med frekvens basert på temperaturen.
 * 
 * Kretsen:
 *   1 x LM35 analog temperatursensor
 *   1 x RGB LED
 *   3 x 220 ohm motstand
 *   1 x buzzer
 * Oppkobling vises på wiki:
 * https://www.ntnu.no/wiki/display/plab/7.+LM35+Temperatursensor
 * 
 * 
 * Reads the temperature with the analogue sensor LM35.
 * Reflects wether the temperature is less or greater than 25 degrees with a light signal.
 * Makes a sound dependent on the temperature.
 * 
 * Circuit:
 *   1 x LM35 analogue temperature sensor
 *   1 x RGB LED
 *   3 x 220 ohm resistor
 *   1 x buzzer
 * How to connect curcuit can be seen here:
 * https://www.ntnu.no/wiki/display/plab/7.+LM35+Temperatursensor
 */
// LM35 Pin Variables
const int sensorPin = A3; //the analog pin the LM35's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade

// Constant LED output pins. These won't change.
const int RGB_bluepin = 4;
const int RGB_redpin = 2;

const int speakerOut = 6; // Put speaker through 220 ohm on pin 6.

int frequency = 0;

void setup()
{
  pinMode(RGB_redpin, OUTPUT);   // sets the pin as output
  pinMode(RGB_bluepin, OUTPUT);   // sets the pin as output  
  pinMode(speakerOut, OUTPUT);
}
 
// Function to produce one square pulse of a
// given frequency on a given pin:
void playOneSquarePulse(int pin, int freq) {
  long int pulseLength = 500000.0 / freq;
  digitalWrite(pin, HIGH);
  delayMicroseconds(pulseLength);
  digitalWrite(pin, LOW);
  delayMicroseconds(pulseLength);
} 
 
void loop()                     // run over and over again
{
  //getting the voltage reading from the temperature sensor
  int reading = analogRead(sensorPin);  
  float voltage = reading * 5.0 / 1024.0; 
  float temperatureC = voltage * 100.0;  //converting from 10 mv per degree                                            //to degrees ((volatge - 500mV) times 100)
  if (temperatureC > 25.0) {
    digitalWrite(RGB_redpin, HIGH); 
    digitalWrite(RGB_bluepin,LOW);}
  else {
    digitalWrite(RGB_redpin, LOW); 
    digitalWrite(RGB_bluepin,HIGH); 
  }
  frequency = (temperatureC - 20.0) * 100.0;
  if (frequency < 100) {
    frequency = 100;
  }
  playOneSquarePulse(speakerOut, frequency);
}
