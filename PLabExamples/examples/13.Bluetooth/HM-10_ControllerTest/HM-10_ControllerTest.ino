/*
 * An example that show how to use the PLabBTLEController library to control functionality of BTLE units.
 * The devices are internally controlled by AT commands. Devices known to work with this library is HM-10 and HM-11.
 * 
 * Circuit:
 *   1 x HM-10 Bluetooth LE module
 *   
 * 
 * https://www.ntnu.no/wiki/pages/viewpage.action?pageId=78024129
 * 
 * Created by Inge Edward Halsaunet, 2015
 * Released into the public domain
 */
// Libraries:
// SoftwareSerial.h is used by PLabBTLEController, and must be included to compile
#include <SoftwareSerial.h>
// PLabBTLEController.h is giving functionality to control BTLE units
#include <PLabBTLEController.h>

const int btUnitTxPin = 2; // Connected to tx on bt unit
const int btUnitRxPin = 3; // Connected to rx on bt unit

// Set up BTLE device controller.
PLabBTLEController controller(btUnitTxPin, btUnitRxPin);

// To keep track of wheter we have told user of the current information
bool toldUser = false;

// A buffer to store received data in, with corresponding size and location identifier
const int bSize = 100;
int bLoc = 0;
char buffer[bSize];

void setup() {
  // Start serial communication with user
  Serial.begin(9600);
  // Zero out buffer
  for (int i = 0; i < bSize; ++i) {
    buffer[i] = 0;
  }
  // Start communication with BTLE device
  Serial.println("Hello, starting communication with BTLE device");
  controller.begin(9600);
  Serial.println("Communication running");
}

void loop() {
  // Updating the data structures of the controller
  controller.update();
  
  // If we have not told the user of the results, and the controller is ready: Tell user
  if (controller.isReady() && !toldUser) {
    toldUser = true;
    
    // Printing name
    Serial.print("Name of device: ");
    Serial.println(controller.getName());
    // Check if the device is set up to work immediately or not
    if (controller.isWorkingImmediately()) {
      Serial.println("This device is working immediately");
    } else {
      Serial.println("This device is NOT working immediately");
    }
    // Check id the device is set up as a central unit
    if (controller.isCentral()) {
      Serial.println("This device is a central device");
    } else {
      Serial.println("This device is NOT a central device");
    }
    // Check if the device is set up as a peripheral unit
    if (controller.isPeripheral()) {
      Serial.println("This device is a peripheral device");
    } else {
      Serial.println("This device is NOT a peripheral device");
    }
    
    // Tell the user how to configure the device
    Serial.println("Now give some input. The controls are as follows:");
    Serial.println("\t.\tGive a new name. Name is the text before the .");
    Serial.println("\t+\tSet working immediately");
    Serial.println("\t_\tSet not working immediately");
    Serial.println("\t,\tSet role as peripheral");
    Serial.println("\t-\tSet role as central");
    Serial.println("\t<\tReset device");
    Serial.println("\t>\tFactory reset device");
  }
  
  // If the device has some data to give to user, pass it on
  if (controller.available()) {
    Serial.write(controller.read());
  }
  
  // If user has inputed some data
  if (Serial.available()) {
    // Read data to buffer
    buffer[bLoc] = Serial.read();
    // We start by assuming the input was not a control
    // However if it turns out it was a command, we will revisit this
    bool controlRcvd = false;
    
    // Now check the last received input against control signals
    switch (buffer[bLoc])
    {
      // Set the name
    case '.':
      // Set the control character at the end of the buffer to be end of string character
      buffer[bLoc] = 0;
      // And set buffer location to restart
      bLoc = 0;
      // Tell the user of the input, and set name of device
      Serial.print("New name should become: ");
      Serial.println(buffer);
      controller.setName(buffer);
      // We have no longer told the user of the changed information
      toldUser = false;
      controlRcvd = true;
      break;
      
      // The device should work immediately
    case '+':
      // Set the control character at the end of the buffer to be end of string character
      buffer[bLoc] = 0;
      // And set buffer location to restart
      bLoc = 0;
      Serial.println("Device should now start working immediately");
      controller.setWorkingImmediately(true);
      toldUser = false;
      controlRcvd = true;
      break;
      
      // The device should not work immediately
    case '_':
      // Set the control character at the end of the buffer to be end of string character
      buffer[bLoc] = 0;
      // And set buffer location to restart
      bLoc = 0;
      Serial.println("Device should now not work immediately");
      controller.setWorkingImmediately(false);
      toldUser = false;
      controlRcvd = true;
      break;
      
      // The device should take peripheral role
    case ',':
      // Set the control character at the end of the buffer to be end of string character
      buffer[bLoc] = 0;
      // And set buffer location to restart
      bLoc = 0;
      Serial.println("Device should become peripheral");
      controller.setRole(PLabBTLEController::PERIPHERAL);
      toldUser = false;
      controlRcvd = true;
      break;
      
      // The device should take central role
    case '-':
      // Set the control character at the end of the buffer to be end of string character
      buffer[bLoc] = 0;
      // And set buffer location to restart
      bLoc = 0;
      Serial.println("Device should become central");
      controller.setRole(PLabBTLEController::CENTRAL);
      toldUser = false;
      controlRcvd = true;
      break;
      
      // We should reset device
    case '<':
      // Set the control character at the end of the buffer to be end of string character
      buffer[bLoc] = 0;
      // And set buffer location to restart
      bLoc = 0;
      Serial.println("Device should now reset");
      controller.reset();
      toldUser = false;
      controlRcvd = true;
      break;
      
      // We should do a factory reset of device
    case '>':
      // Set the control character at the end of the buffer to be end of string character
      buffer[bLoc] = 0;
      // And set buffer location to restart
      bLoc = 0;
      Serial.println("Device should now factory reset");
      controller.factoryReset();
      toldUser = false;
      controlRcvd = true;
      break;
    }
    
    // And now, if we did not receive command, increment position in buffer
    if (!controlRcvd) {
      bLoc++;
      // And make sure we do not overstep the buffer
      if (bLoc >= bSize) {
        bLoc = bSize - 1;
      }
    }
  }
}
