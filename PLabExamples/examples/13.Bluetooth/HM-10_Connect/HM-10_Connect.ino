/*
 * HM-10_Connect
 * 
 * An example that show how to use the PLabBTLEController library to connect two BTLE devices.
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

// If the control screen should be redrawn
bool reprintScreen = true;

void setup() {
  // Starting serial communication with console
  Serial.begin(9600);
  // Starting serial communication with BTLE device
  Serial.println("Hello. Starting communication with BTLE device");
  controller.begin(9600);
  Serial.println("Communication running, reading device state");
  // Wait loop until device information is read
  while (!controller.isReady()) { controller.update(); }
  
  // Show user name of device
  Serial.print("Device ");
  Serial.print(controller.getName());
  Serial.println(" read. Setting up for central role");
  
  // Start setting up the device for central role
  // We do not want to connect to the first discovered device
  if (controller.isWorkingImmediately()) {
    controller.setWorkingImmediately(false);
  }
  // Allways wait until device is ready before doing anything
  while (!controller.isReady()) { controller.update(); }
  // We need the central role to discover devices
  if (!controller.isCentral()) {
    controller.setRole(PLabBTLEController::CENTRAL);
  }
  while (!controller.isReady()) { controller.update(); }
  // And in this example we want to discover names of nearby devices
  if (!controller.isDiscoveringNames()) {
    controller.setDiscoveringNames(true);
  }
  while (!controller.isReady()) { controller.update(); }
  Serial.println("Setup done. Do first scan");
  // Short delay before we start scanning for devices
  delay(500);
  controller.discoverDevices();
}

void loop() {
  // Allways allow controller to update its data structures.
  controller.update();
  
  // If the controller is ready, and we should print information to user
  if (controller.isReady() && reprintScreen) {
    reprintScreen = false;
    
    // Check how many devices are found
    int numberDevices = controller.devices();
    Serial.print("Found ");
    Serial.print(numberDevices);
    Serial.println(" devices:");
    Serial.println("\tID\tAddress\t\tName");
    
    // Display the found devices information
    for (int i = 0; i < numberDevices; ++i) {
      // The device
      PLabBTLEDevice dev = controller[i];
      // Address and name. They may be null pointers
      const char *addr = dev.getAddress();
      const char *name = dev.getName();
      // \t is text formatting only
      Serial.print("\t");
      // Show the id of the device
      Serial.print(dev.getId());
      Serial.print("\t");
      // If address is not null, print it
      if (addr == 0) {
        Serial.print("\t\t");
      } else {
        Serial.print(addr);
      }
      // If name is not null, print it
      if (name == 0) {
        Serial.println("\t?");
      } else {
        Serial.print("\t");
        Serial.println(name);
      }
    }
    
    if (controller.isConnected()) {
      // If controller is connected, tell user how to disconnect
      Serial.println("Already connected to device");
      Serial.println("To disconnect, use symbol |");
    } else {
      // If controller is not connected, tell user how to rescan
      Serial.println("To rescan, use symbol |");
      
      // And if we have found devices, tell user how to connect
      if (numberDevices > 0) {
        Serial.print("Select device (0-");
        Serial.print(numberDevices-1);
        Serial.println("). to connect to");
      }
    }
    
  }
  
  // If we have received some input, we should check it for commands
  if (Serial.available()) {
    // Check if the controller is ready to receive any command
    if (!controller.isReady()) {
      Serial.print("Device not ready. Discarded ");
      Serial.println((char)Serial.read());
    } else {
      
      // Read the input and check if it is a command.
      char inp = Serial.read();
      if (inp == '|') {
        // It was a command, andwe should reprint the screen on next device ready run
        reprintScreen = true;
        
        // If we were connected, we should disconnect. Otherwise we should rescan
        if (controller.isConnected()) {
          Serial.println("Disconnecting...");
          controller.disconnect();
        } else {
          Serial.println("Rescanning...");
          controller.discoverDevices();
        }
      } else {
        
        // Input was not a main command. If we were connected, check if it was a device identifier
        int v = inp - '0';
        if (!controller.isConnected() && v >= 0 && v <= controller.devices()) {
          // It was a device identifier, and we should connect
          Serial.println("Connecting to device");
          controller.connectDevice(controller[v]);
          reprintScreen = true;
        } else {
          // It was not a device identifier, and we should send the input to the bluetooth device
          controller.write(inp);
        }
      }
    }
  }
  
  // If some information has come from the device, we should print it to the user.
  if (controller.available()) {
    Serial.write(controller.read());
  }
}
