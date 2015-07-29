/*
 * SensorToPortExpanderLEDsAdvanced
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

unsigned int readings[6];
const int outPorts[] = { 5, 4, 3, 2, 1, 0 }; // Outputs are written in opposite order than they are read to show difference from SensorToPortExpanderLEDs example.
uint8_t outMask;


/**
 * Helper function that initialize a MCP23008 port expander with the given address, with the IO ports that should be used for analog write as outputs.
 *   peAddr      : Address of the port expander
 *   outputPorts : The ports that are to be used as outputs
 *   outputs     : Number of ports used.
 *   portExp     : The port expander. Passed by reference, no copies are made of the proxy object.
 * 
 *   returns     : Bitmask for the used ports
 */
uint8_t portExpander_beginAnalog(int peAddr, const int *outputPorts, int outputs, Adafruit_MCP23008 &portExp) {
  // Init mask
  uint8_t mask = 0;
  // Start communication with port expander
  portExp.begin(peAddr);
  // Set given pins as outputs
  for (int i = 0; i < outputs; ++i) {
    mask |= 0x1 << outputPorts[i]; // Binary or the mask with 1 at the location of this port
    portExp.pinMode(outputPorts[i], OUTPUT);
  }
  
  return mask;
}

/**
 * Helper function that simulate 1 cycle of PWM written analog output.
 *   values      : Array of values. Size of array MUST be AT LEAST as big as number of ports used. Values are expected in range 0 - 100
 *   outputPorts : The ports that are used as outputs
 *   outputs     : Number of ports used.
 *   outputMask  : The bit mask for the output as returned by portExpander_beginAnalog()
 *   portExp     : The port expander. Passed by reference, no copies are made of the proxy object.
 */
void portExpander_analogWrite(unsigned int *values, const int *outputPorts, int outputs, uint8_t outputMask, Adafruit_MCP23008 &portExp) {
  
  // Expected value range: 0 - MAX_VALUE
  const int MAX_VALUE = 100;
  // Step size: Determines resolution. Too high resolution (low step size) may result in flashing lights
  const int STEP_SIZE = 10;
  
  // Read values that that are currently on the output, and mask them with the opposite of the output mask.
  // This will be used with all writes.
  uint8_t prevVals = portExp.readGPIO() & ~outputMask;
  
  for (unsigned int i = 0; i <= MAX_VALUE; i += STEP_SIZE) {
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
    for (int s = 0; s < outputs; ++s) {
      
      // Binary manipulation:
      val |= (readings[s] < i) << outputPorts[s];   // this line can be translated to:
      // uint8_t current = readings[s] < i;         // Comparison will in c/c++ return 1 for true and 0 for false.
      // current = current << outputPorts[s];       // left shift the compared value output port number times. As s is our sensor number, this means moving the compared value to its correct position in the byte (move it output port number steps to the left).
      // val = val | current;                       // Binary or the two values together.
      
    }
    // After this operation, the uint8_t now holds values (0 or 1) at the positions indicated by outputPorts.
    // To make sure this is in the correct position, and that we do not alter the other IO ports, we binary and this with the output mask and binary or the result with the values that were there before
    val = (val & outputMask) | prevVals;
    
    // To send the entire data in one operation, we call:
    portExp.writeGPIO(val);
  }
  
  // Make sure all lights are turned off while we do other things
  portExp.writeGPIO(prevVals);
}


void setup() {
  // Console serial communication
  Serial.begin(9600);
  Serial.println("Hello. Initializing zumo and port expander");
  
  // Start communication with port expander and init the 6 output ports used for analog write
  outMask = portExpander_beginAnalog(addr, outPorts, 6, mcp);

  // Initialize sensor array
  sensors.init();
  
  Serial.println("Done. Starting the light");
}



void loop() {
  // Read all sensor values
  sensors.read(readings);
  
  // From the documentation for the Zumo library we know that sensor values read are in range 0 - 2000
  // We wish to bound these values in range 0 - 100. Only 6 values are read.
  for (int i = 0; i < 6; ++i) {
    readings[i] /= 20;
  }
  
  // Do one cycle of analog write on the given ports
  portExpander_analogWrite(readings, outPorts, 6, outMask, mcp);
}
