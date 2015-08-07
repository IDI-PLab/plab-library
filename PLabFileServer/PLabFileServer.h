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
// Recommended size is over 100 (to avoid too long URI). MUST be AT LEAST 70 to hold error code and neccessary html during error generation.
#define PLAB_BIG_BUFFER_SIZE 128
#endif // ndef PLAB_BIG_BUFFER_SIZE

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


	void cPrint(const char *str, EthernetClient &c);
	void cPrintln(const char *str, EthernetClient &c);
	void cWrite(char chr, EthernetClient &c);

	RequestMethod_t acceptRequestMethod();
	bool acceptRequestURI();
	bool acceptHTTPVersion();
	bool acceptHeader(bool complete);

	File sdFile;

	char mimeSufBuf[PLAB_MIME_SUFFIX_BUF_MAX];
	int internalMIMEIndex = -1;

	char bigBuf[PLAB_BIG_BUFFER_SIZE];

	bool sdInitialized = false;

	RequestState_t reqStateTransition(RequestState_t oldState, char recvd, int &recCount, RequestMethod_t &method);

	int internalMIMEType(const char *suffix);

	void printDefaultError(int errorCode, EthernetClient &client);
public:
	Stream *out = NULL;

	PLabFileServer() : EthernetServer(80) {}
	PLabFileServer(int port) : EthernetServer(port) {}
	~PLabFileServer() {}

	void begin();
	void update();
};

#endif //ndef PLAB_FILE_SERVER_H
