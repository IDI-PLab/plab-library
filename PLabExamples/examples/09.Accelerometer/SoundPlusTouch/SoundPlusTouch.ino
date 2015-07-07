/*
 * SoundPlusTouch
 * 
 * Leser sensorverdiene til et akselerometer. 
 * En lyd spilles basert på verdien så lenge en touch sensor er aktiviert.
 * 
 * Kretsen:
 *   1 x analog akselerometer ADXL335B
 *   1 x 220 ohm motstand
 *   1 x buzzer
 *   1 x touch sensor
 * Oppkoblingen vises på wiki:
 * https://www.ntnu.no/wiki/display/plab/6.+Akselerometer+ADXL335B
 * 
 * 
 * Reads the sensor value of an accelerometer. 
 * A sound is played based on the value as long as a touch sensor is activated.
 * 
 * Circuit:
 *   1 x analogue accelerometer ADXL335B
 *   1 x 220 ohm resistor
 *   1 x buzzer
 *   1 x touch sensor
 * How to connect curcuit can be seen here:
 * https://www.ntnu.no/wiki/display/plab/6.+Akselerometer+ADXL335B
 */
// these constants describe the pins. They won't change:
const int xpin = A0;                  // x-axis of the accelerometer
const int ypin = A1;                  // y-axis
const int zpin = A2;                  // z-axis (only on 3-axis models)

const int speakerOut = 6; // Put speaker through 220 ohm on pin 6.
int frequency = 0;

const int touchInput = 5; // Put touchinput on pin 5
int touchValue = 0;

int xValue = 0;
int yValue = 0;
int zValue = 0;

void setup()
{
  pinMode(speakerOut, OUTPUT);
  pinMode(touchInput, INPUT);
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

void loop()
{
  xValue = analogRead(xpin);
  yValue = analogRead(ypin); 
  zValue = analogRead(zpin); 
  frequency = xValue + yValue + zValue;
  frequency = (frequency - 600)*5;
  touchValue = digitalRead(touchInput);
  if (touchValue == HIGH) {
     playOneSquarePulse(speakerOut, frequency);
  }
}
