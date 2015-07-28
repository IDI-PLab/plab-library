/*
 * SensorToPortExpanderLEDs
 *
 * Leser av verdiene fra Zumos refleksjonssensorer, tolker signalet
 * som et analogt signal og lyser en LED per sensor på en port
 * expander. Lysets intensitet er bestemt av styrken på avlest
 * signal.
 *
 * Kretsen:
 *   1 x Zumo med sensor array
 *   6 x LED
 *   6 x 220 ohm motstand
 *   1 x 10 k ohm motstand
 * Oppkobling:
 * https://www.ntnu.no/wiki/display/plab/9.+Port+Expander
 *
 *
 * Reads values from the Zumo reflection sensors, interprets the
 * signal as an analog signal and shines one LED per sensor. The
 * LEDs should be connected to a port expander. The light intensity
 * is determined by signal strength.
 *
 * Circuit:
 *   1 x Zumo with reflectance senosr array
 *   6 x LED
 *   6 x 220 ohm resistor
 *   1 x 10 k ohm resistor
 * Connecting the circuit:
 * https://www.ntnu.no/wiki/display/plab/9.+Port+Expander
 */

// inttypes holds the uint8_t (unsigned integer, 8 bit) type
#include <inttypes.h>
// Zumo library includes
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
// Port expander includes
#include <Wire.h>
#include <Adafruit_MCP23008.h>

const int addr = 0;  // Port expander address
Adafruit_MCP23008 mcp;  // Port exander proxy

ZumoReflectanceSensorArray sensors; // Sensor array proxy
const unsigned int maxReading = 2000;  // Maximum value that can be read from sensors (defined in library)

unsigned int readings[6];  // Will hold the read sensor values

void setup() {
  // Console serial communication
  Serial.begin(9600);
  Serial.println("Hello. Initializing zumo and port expander");
  
  // Start communication with port expander
  mcp.begin(addr);
  // Set all pins as outputs
  for (int i = 0; i < 8; ++i) {
    mcp.pinMode(i, OUTPUT);
  }

  // Initialize sensor array
  sensors.init();
  Serial.println("Done. Starting the light");
}

void loop() {
  // Read all sensor values
  sensors.read(readings);
  
  // To get the lights to seem to have different intensities, we can
  // blink them very fast. As i2c is quite sluggish on Arduino, we
  // set the increment to be high, here 50.
  for (unsigned int i = 0; i < maxReading; i += 50) {
    
    // To speed up i2c communication, we only send one update signal
    // each round.
    
    // On our port expander we have 8 IO pins. We can represent all
    // a single byte. We start to build this by initializing it to 0
    uint8_t val = 0;
    // we use the unsigned integer 8 bit type as we shall hold data
    // for 8 port (1 bit per port) and representing it as a signed
    // value does not make sense.
    
    // The port numbers and the binary values it holds is now:
    // port num: 7 6 5 4 3 2 1 0
    // value:    0 0 0 0 0 0 0 0
    
    // we can now loop through all readings to update the byte
    // representation of our outputs
    for (int s = 0; s < 6; ++s) {
      
      // Binary manipulation:
      val |= (readings[s] < i) << s;         // this line can be translated to:
      // uint8_t current = readings[s] < i;  // Comparison will in c/c++ return 1 for true and 0 for false.
      // current = current << s;             // left shift the compared value s times. As s is our sensor number, this means moving the compared value to its correct position in the byte (move it s steps to the left).
      // val = val | current;                // Binary or the two values together.
      
    }
    
    // After this operation, the uint8_t now holds
    // port num: 7 6 5 4 3 2 1 0
    // value:    0 0 x x x x x x
    // where 'x' represents the result of the comparison for that particular sensor
    
    // To send the entire data in one operation, we call:
    mcp.writeGPIO(val);
    // The port expander librart will use some time on completing this request, so no delay at the end of the loop
  }
}
