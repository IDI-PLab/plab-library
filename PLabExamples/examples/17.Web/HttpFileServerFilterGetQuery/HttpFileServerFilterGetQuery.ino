/*
 * HttpFileServerFilterGetQuery
 * 
 * A simple example that show how use PLabFileServer with a filter to get the GET query string.
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

// To change responses, set up a filter.
class MyFilter : public PLabServerFilter {
public:
  bool filterRequestUri(char *uri) {
    char *queryString = strchr(uri, '?');
    if (queryString) {
      // Remove the leading '?'
      queryString = &(queryString[1]);
      Serial.println("Queried for:");
      Serial.println(queryString);
    }
    return false;
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
