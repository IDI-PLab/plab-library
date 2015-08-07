/*
* PLabFileServer
* Version 0.1, August, 2015
* This library for the Arduino Uno simplify use of the Ethernet library
* as a http file server.
*
* Created by Inge Edward Halsaunet, 2015
* Released into the public domain
*/
#ifndef PLAB_FILE_SERVER_H
#define PLAB_FILE_SERVER_H

#include "PLabServerStrings.h"

#include <avr/pgmspace.h>
#include <SPI.h>	
#include <Ethernet.h>
#include <Stream.h>
#include <SD.h>

#ifndef PLAB_BIG_BUFFER_SIZE
// Big buffer, all incomming and a lot of outgoing is being buffered here.
// Recommended size is over 80 (to avoid too long URI). MUST be AT LEAST 70 to hold error code and neccessary html during error generation.
#define PLAB_BIG_BUFFER_SIZE 80
#endif // ndef PLAB_BIG_BUFFER_SIZE

class PLabServerFilter {
public:
	virtual void start() {}
	virtual bool filterRequestUri(char *uri) { return false; }
	virtual bool filterRequestHeader(char *content, bool complete) { return true; }
	//virtual void acceptRequestBody(char character) {}	// Not yet relevant, no requests should have body
	virtual void redirectReplyFile(File &f) {}
	virtual void writeResponse(EthernetClient &client) {}
	virtual void filterResponseHeader(char *response, EthernetClient &client) {}
	virtual bool filterResponse(char character, EthernetClient &client) { return true; }
	virtual void end() {}
};

class PLabFileServer : private EthernetServer {
private:
	enum RequestState_t {
		REQ_START,
		REQ_LINE_METHOD, REQ_LINE_URI, REQ_LINE_HTTP_V, REQ_LINE_END,
		REQ_HEAD, REQ_HEAD_CR_ACC, REQ_HEAD_LF_ACC,
		EMPTY_LINE,
		REQ_MESSAGE_BODY,

		BAD_REQ_400,					// Syntax error in request
		NOT_FOUND_404,					// 404
		METHOD_NOT_ALLOWED_405,			// on that particular resource
		// REQUEST_TIMEOUT_408,			// client did not produce request in the time the server was willing to wait.
		// REQUEST_ENTITY_TOO_LARGE_413	// request larger that the server can or will accept
		REQUEST_URI_TOO_LONG_414,		// We ran out of buffer
		INTERNAL_SERVER_ERROR_500,		// Internal server error
		NOT_IMPLEMENTED_501,			// Method not implemented (MUST accept GET and HEAD)
		HTTP_VERSION_NOT_SUPPORTED_505	// Major version
	};
	enum RequestMethod_t {
		METH_NOT_SUPPORTED, METH_GET, METH_HEAD
	};		// Supported request methods

	// If the filter is responsible for writing the response
	bool userControlledResponse = false;


	// cPrint, cPrintln, cWrite : wrappers that simplify debug printing.
	void cPrint(const char *str, EthernetClient &c);
	void cPrintln(const char *str, EthernetClient &c);
	void cWrite(char chr, EthernetClient &c);

	// called when that part of a request has completed
	RequestMethod_t acceptRequestMethod();	// Accepted: GET / HEAD , not accepted POST etc.
	bool acceptRequestURI();	// The complete uri has been received, including query
	bool acceptHTTPVersion();	// Check if HTTP version is compatible
	bool acceptHeader(bool complete);	// Actually only one line of the header. complete is if the line is completed.

	File sdFile;	// While an open request exists, this holds the open sd file.

	char mimeSufBuf[PLAB_MIME_SUFFIX_BUF_MAX];	// MIME type is determined by suffix. This is used when reading this suffix
	int internalMIMEIndex = -1;	// Where in progmem table MIME type is stored

	char bigBuf[PLAB_BIG_BUFFER_SIZE];	// Buffer used for both input and outpur. Used extensively to read from progmem

	bool sdInitialized = false;	// If SD initialization succeeded

	RequestState_t reqStateTransition(RequestState_t oldState, char recvd, int &recCount, RequestMethod_t &method);	// While parsin request, this is the state macine "engine"

	int internalMIMEType(const char *suffix);	// Query where a suffix is located in the MIME type progmem table. Returns -1 if not found

	void printDefaultError(int errorCode, EthernetClient &client);	// Print the default error html to the client. Error code and message string is found from erorCode
public:
	Stream *out = NULL;		// PUBLIC -> User of class can change it him/herself
	PLabServerFilter *filter = NULL;	// PUBLIC -> User of class can set filter (if wanted)

	PLabFileServer() : EthernetServer(80) {}	// Server defaults to port 80
	PLabFileServer(int port) : EthernetServer(port) {}	// Custom port
	~PLabFileServer() {}

	void begin();	// Init SD card, and calls EthernetServer::begin()
	void update();	// Checks for new connections
};

#endif //ndef PLAB_FILE_SERVER_H
