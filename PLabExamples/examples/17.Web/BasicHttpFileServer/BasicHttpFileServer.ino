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
//  Serial.println("Setting up a basic file server");
  // Giving server debug
  server.out = &Serial;
  
  // Obtaining IP address from DHCP
  Ethernet.begin(mac);
//  Serial.print("Server IP: ");
  Serial.println(Ethernet.localIP());

  // Starting server
//  Serial.println("Starting server");
  server.begin();
  Serial.println("Started");
}

void loop() {
  server.update();
}
