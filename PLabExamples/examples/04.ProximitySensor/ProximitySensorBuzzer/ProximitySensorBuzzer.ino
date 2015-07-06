
/*
  ProximitySensorBuzzer
  
  Dette er et eksempel på hvordan du kan bruke en HC-SR04 ultralyd sensor med biblioteket NewPing for å måle avstand.
  Målt avstand brukes for å bestemme frekvensen til avspillt tone.
  
  Kretsen:
    1 x HC-SR04 (ultralyd sensor)
    1 x 220 ohm motstand
    1 x buzzer
  Oppkobling vises på wiki:
  https://www.ntnu.no/wiki/display/plab/3.+Ultralyd+-+HC-SR04
  
  Bibliotek
    - NewPing (Nedlasting: https://code.google.com/p/arduino-new-ping/downloads/list )
  
  
  This is an example on how you may use a HC-SR04 ultrasonic sensor with the library NewPing to detect distance.
  Distance measured is used to determine frequency of played tone.
  
  Circuit
    1 x HC-SR04 (ultrasonic sensor)
    1 x 220 ohm resistor
    1 x buzzer
  How to connect circuit can be seen here:
  https://www.ntnu.no/wiki/display/plab/3.+Ultralyd+-+HC-SR04
  
  Library
    - NewPing (Download: https://code.google.com/p/arduino-new-ping/downloads/list )
 */
#include <NewPing.h>

const int echoPin = 7;
const int triggerPin = 8;
const int maxDistance = 100;  // default 300 
NewPing sonar(triggerPin, echoPin, maxDistance);

const int speakerOut = 9; // Put speaker through 220 ohm on pin 9.
int frequency = 0;
int duration = 200;

void setup() {
   pinMode(speakerOut, OUTPUT);
}

// Funskjon for å produsere en firkantpuls med gitt frekvens på gitt pinne.
// Function to produce one square pulse of a
// given frequency on a given pin:
void playTone(int pin, int freq, int dur) {
  long int pulseLength = 500000 / freq;
  long int durLong = dur;
  long int noOfPulses = (durLong*1000) / (pulseLength*2);
  for (long int i = 0; i < noOfPulses; i++) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(pulseLength);
    digitalWrite(pin, LOW);
    delayMicroseconds(pulseLength);
  }
}


void loop() {
  // Gjør ett ping, og beregn avstanden
  // Do ping and calculate distance
  unsigned int time = sonar.ping();
  float distance = sonar.convert_cm(time);

  // Beregn frekvens og spill av lyd
  // Calculate frequency and play sound
  frequency = 30 * distance;
  playTone(speakerOut, frequency, 200); 
  delay(50);
}
