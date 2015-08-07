/*
 * HttpFileServerFilterTest
 * 
 * A simple example that show how use PLabFileServer with a filter to change response text.
 * A string is inserted every time "test" is read.
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

const char respondTo[] = "test";

// To change responses, set up a filter.
class MyFilter : public PLabServerFilter {
private:
  int i = 0;

public:
  void start() {
    i = 0;
  }
  bool filterResponse(char character, EthernetClient &client) {
    if (character != respondTo[i]) {
      i = 0;
    } else {
      i++;
      if ('\0' == respondTo[i]) {
        client.print(character);
        client.print(" INSERTED BY FILTER ");
        return false; // We have already written this character
      }
    }
    return true;
    // Called on each character we are writing to the client
  }
};

byte mac[] = {
  0x90, 0xA2, 0xDA, 0x0E, 0xCD, 0xE3
};

PLabFileServer server;  // Defaults to port 80
// PLabFileServer server(3030);  // For a specific port

MyFilter myFilter;  // Global scope, we will use pointer to it!

void setup() {
  Serial.begin(9600);
  // Giving server debug
  //server.out = &Serial; // No cluttering of output, we want only filters

  // Adding filter to the server
  server.filter = &myFilter;
  
  // Obtaining IP address from DHCP
  while (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP fail");
    delay(10000);
  }
  
  Serial.println(Ethernet.localIP());

  server.begin();
}

void loop() {
  server.update();
}
