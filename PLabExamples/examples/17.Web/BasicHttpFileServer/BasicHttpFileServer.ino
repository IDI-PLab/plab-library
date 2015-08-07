/*
 * BasicHttpFileServer
 * 
 * A simple example that show how use PLabFileServer.
 * The server serves files from an SD card connected to the ethernet shield.
 * All request/reply data is also fed to Serial.
 * 
 * Circuit:
 *   1 x Arduino Ethernet shield
 * 
 * Created by Inge Edward Halsaunet, 2015
 * Released into the public domain
 */
#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>
#include <PLabFileServer.h>

byte mac[] = {
  0x90, 0xA2, 0xDA, 0x0E, 0xCD, 0xE3
};

PLabFileServer server;  // Defaults to port 80

void setup() {
  Serial.begin(9600);
  Serial.println("Setting up a basic file server");
  // Giving server debug
  server.out = &Serial;
  
  // Obtaining IP address from DHCP
  while (Ethernet.begin(mac) == 0) {
    Serial.println("A problem occured while aquire IP");
    delay(10000);
  }
  Serial.print("Server IP: ");
  Serial.println(Ethernet.localIP());

  // Starting server
  Serial.println("Starting server");
  server.begin();
  Serial.println("Started");
}

void loop() {
  server.update();
}
