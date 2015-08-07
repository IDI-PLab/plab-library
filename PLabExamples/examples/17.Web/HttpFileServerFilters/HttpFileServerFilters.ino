/*
 * HttpFileServerFilters
 * 
 * An example that show how use PLabFileServer with a filter.
 * The filter does not change anything.
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
// Implement one or more of the following methods
class MyFilter : public PLabServerFilter {
public:
  void start() {
    Serial.println("Start");
    // Called  when new request is accepted
    // If filter has state for each connection, set it up here
  }
  bool filterRequestUri(char *uri) {
    Serial.print("URI: ");
    Serial.println(uri);
    // return true;   // If you wish to write the entire response yourself
    return false;
  }
  bool filterRequestHeader(char *content, bool complete) {
    Serial.print("Header: ");
    Serial.print(content);
    if (complete) {
      Serial.println(" : Accepted");
    }
    // return false;  // If you want to reply with 400 Bad Request
    return true;
  }
  void redirectReplyFile(File &f) {
    Serial.println("Redirect file");
    // If for some reason you wish to change the file that is being sent in the reply, change file f here
  }
  void writeResponse(EthernetClient &client) {
    // This is ONLY called with requests where you returned true in filterRequestUri()
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<html><head></head><body>Empty</body></html>");
  }
  void filterResponseHeader(char *response, EthernetClient &client) {
    // Called while replying, before actual content is sent
    Serial.print("Filter response header: ");
    Serial.println(response);
  }
  bool filterResponse(char character, EthernetClient &client) {
    Serial.print(character);
    // return false;  // If you do not wish the server to write that character to the client
    return true;
    // Called on each character we are writing to the client
  }
  void end() {
    Serial.println("End");
    // Called when client disconnects
    // If filter has state, do cleanup here
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
