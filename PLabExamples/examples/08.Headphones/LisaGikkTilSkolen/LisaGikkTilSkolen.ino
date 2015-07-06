/*
 * LisaGikkTilSkolen
 * 
 * Spiller begynnelsen av sangen "Lisa gikk til skolen" i loop.
 * 
 * ADVARSEL:
 * Pass på hørselen. Vær sikker på du bruker stor nok motstand i serie med hodetelefonene.
 * Test ALLTID med hodetelefonene vekk fra ørene først!
 * 
 * Kretsen:
 *   1 x stereo 3,5 mm jack hunn plugg
 *   1 x 10 k ohm motstand
 *   1 x hodetelefoner
 *   valgfri: 1 x 10/100 k ohm variabel motstand
 * Oppkobling kan finnes på wiki:
 * https://www.ntnu.no/wiki/display/plab/6.+Hodetelefoner
 * 
 * 
 * Plays the beginning of the song "Lisa gikk til skolen" in a loop.
 * 
 * WARNING:
 * Take care of your hearing. Make sure you use a large enough resistor in series with your headset.
 * ALLWAYS test with the headphones away from your ears before putting them close to your ears.
 * 
 * Circuit:
 *   1 x stereo 3.5 mm jack female
 *   1 x 10 k ohm resistor
 *   1 x headset
 *   optional: 1 x 10/100 k ohm potentiometer
 * How to connect the circuit can be seen here:
 * https://www.ntnu.no/wiki/display/plab/6.+Hodetelefoner
 */
// TONES  ==========================================
#define  c4     262 
#define  d4     294 
#define  e4     330 
#define  f4     349 
#define  g4     392 
#define  a4     440 
#define  b4     494 
#define  c5     523 
// Define a special note, 'R', to represent a rest
#define  R     0

const int speakerOut = 8; // The pin we connect the resistor and headphones to

// MELODY and TIMING  =======================================
//  melody[] is an array of notes, accompanied by beats[],
int melody[] = {  c4,  d4,  e4,  f4,  g4,   g4,  a4,  a4,  a4,  a4, g4 };
int beats[]  = { 4, 4, 4, 4, 2, 2, 4, 4, 4, 4, 1 };
int MAX_COUNT = 11; // Melody length, for looping.

long tempo = 1000;  // Set overall tempo
long pauseTime = tempo/25;  // Set length of pause between notes
int frequency = 0;  
int beat = 0;
long duration  = 0;

void setup() {
  pinMode(speakerOut, OUTPUT);
}

void loop() {
  // Set up a counter to pull from melody[] and beats[]
  for (int i=0; i<MAX_COUNT; i++) {
    frequency = melody[i];
    beat = beats[i];
    duration = tempo / beat; // Set up timing
    tone(speakerOut, frequency, duration);
    delay(duration);
    delay(pauseTime); // A pause between notes...
  }
  delay(500); // A pause between melodies.
}

