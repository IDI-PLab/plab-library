//#define PLAB_DEBUG

#include "PLabFileServer.h"

char *findLast(const char *str, char c) {
	char *last = NULL;
	char *next = strchr(str, c);
	while (next) {
		last = &(next[1]);
		next = strchr(last, c);
	}
	return last;
}

void PLabFileServer::cPrint(const char *str, EthernetClient &c) {
	if (out)
		out->print(str);
	c.print(str);
}
void PLabFileServer::cPrintln(const char *str, EthernetClient &c) {
	if (out)
		out->println(str);
	c.println(str);
}
void PLabFileServer::cWrite(char chr, EthernetClient &c) {
	if (out)
		out->write(chr);
	c.write(chr);
}

int PLabFileServer::internalMIMEType(const char *suffix) {
	// Find index, unordered search
	for (int i = 0; i < (sizeof(plabMIMESuffixTable) / sizeof(char*)); ++i) {
		// Get the mime suffix from prgram memory
		strcpy_P(mimeSufBuf, (char*)(pgm_read_word(&(plabMIMESuffixTable[i]))));
		if (strcasecmp(mimeSufBuf, suffix))
			return i;
	}
	return -1;
}
int PLabFileServer::userMIMEType(const char *suffix) {
	// TODO Treat user defined MIME type
	return -1;
}
 void PLabFileServer::bufferMIMEType(const char *suffix) {
	 // ensure MIME type defaults to unknown
	 mimeTypeBuf[0] = '\0';

	int index = userMIMEType(suffix);
	if (index >= 0) {
		// TODO Treat user defined MIME type
	}
	else {
		index = internalMIMEType(suffix);
		if (index >= 0) {
			// Copy type from program memory
			strcpy_P(mimeTypeBuf, (char*)(pgm_read_word(&(plabMIMETypeTable[index]))));
		}
	}
}


void PLabFileServer::begin() {
	// Initialization should be done here
	EthernetServer::begin();

	if (!sdInitialized) {
		sdInitialized = SD.begin(4);
		if (!sdInitialized && out) {
			out->println("SD card initialization failed!");
		}
	}
}

// ---------------------------------------------------------------------------
// -------------- Request parsing --------------------------------------------
// ---------------------------------------------------------------------------
/*
METH_NOT_SUPPORTED, METH_GET, METH_HEAD
*/
PLabFileServer::RequestMethod_t PLabFileServer::acceptRequestMethod() {
	if (strcmp(bigBuf, "GET") == 0) {
		return METH_GET;
	}
	else if (strcmp(bigBuf, "HEAD") == 0) {
		return METH_HEAD;
	}
	return METH_NOT_SUPPORTED;
}
bool PLabFileServer::acceptRequestURI() {
	// First off, SD card must be initialized
	if (!sdInitialized)
		return false;

	// 0 Ensure file is closed right now, and MIME is unknown
	if (sdFile) {
		sdFile.close();
	}

	char *uri = bigBuf;

	// 1 Send URI to user, if user wish to check it
	// 2 check for URI type
	// 3 clean URI
	if (uri[0] == '/'){
		uri = &(uri[1]);
	}
#ifdef PLAB_DEBUG
	if (out)
		out->println(uri);
#endif // PLAB_DEBUG
	// 4 Check for existence of file (after user has done modifications)
	if (SD.exists(uri)) {
		// 5 Do we understand the MIME type?
		char *suf = findLast(uri, '.');
#ifdef PLAB_DEBUG
		if (out)
			out->println(suf);
#endif // PLAB_DEBUG
		bufferMIMEType(suf);
		// 6 Open the file, and let it wait until we have a complete request
		sdFile = SD.open(bigBuf, FILE_READ);
		return true;
	}
	return false;

	// TODO This is probably a really bad way to do things. Some testing should be performed.
	// Check if # (fragment id) or ? (query string) is inside string
	/*
	char* fragPos = strchr(bigBuf, '#');
	char* queryPos = strchr(requestBut, '?');
	char* endPos = nullptr;
	if (fragPos != nullptr && queryPos != nullptr) {
		endPos = fragPos < queryPos ? fragPos;
	}
	else if (fragPos != nullptr) {
		endPos = fragPos;
	}
	else if (queryPos != nullptr) {
		endPos = queryPos;
	}
	*/
	// TODO Do something with the string after ? or #
	// TODO Somehow remember file position (we should not send it before we know request is completely accepted)
}
bool PLabFileServer::acceptHTTPVersion() {
	return !(bigBuf[0] != 'H' || bigBuf[1] != 'T' || bigBuf[2] != 'T' || bigBuf[3] != 'P' || bigBuf[4] != '/'
		|| bigBuf[5] != '1' || bigBuf[6] != '.'); // ONLY major version 1 accepted
}
bool PLabFileServer::acceptHeader(bool complete) { return true; }	// Not yet implemented anything.

/*
REQ_START,
REQ_LINE_METHOD, REQ_LINE_URI, REQ_LINE_HTTP_V, REQ_LINE_END,
REQ_HEAD, REQ_HEAD_END,
REQ_MESSAGE_BODY,

BAD_REQ_400,					// Syntax error in request
NOT_FOUND_404,					// 404
METHOD_NOT_ALLOWED_405,			// on that particular resource
REQUEST_URI_TOO_LONG_414,		// We ran out of buffer
INTERNAL_SERVER_ERROR_500,		// Internal server error
NOT_IMPLEMENTED_501,			// Method not implemented (MUST accept GET and HEAD)
HTTP_VERSION_NOT_SUPPORTED_505	// Major version
*/
PLabFileServer::RequestState_t PLabFileServer::reqStateTransition(RequestState_t oldState, char recvd, int &recCount, RequestMethod_t &method) {

	// If we do not change the state anywhere, the old state should be kept
	RequestState_t nextState = oldState;

	switch (oldState) {

	case REQ_START:
		// Fallthrough

	case REQ_LINE_METHOD:
		if (recvd == ' ') {
			bigBuf[recCount] = '\0';
			recCount = 0;
			method = acceptRequestMethod();
			nextState = (method == METH_NOT_SUPPORTED) ? NOT_IMPLEMENTED_501 : REQ_LINE_URI;
		}
		else
			bigBuf[recCount++] = recvd;
		break;
	case REQ_LINE_URI:
		if (recvd == ' ') {
			bigBuf[recCount] = '\0';
			recCount = 0;
			nextState = acceptRequestURI() ? REQ_LINE_HTTP_V : NOT_FOUND_404;
		}
		else {
			// special case treatment
			bigBuf[recCount++] = recvd;
			if (recCount >= PLAB_BIG_BUFFER_SIZE) {	// We do not have space for the null terminating character
				bigBuf[0] = '\0';
				recCount = 0;
				nextState = REQUEST_URI_TOO_LONG_414;
			}
		}
		break;
	case REQ_LINE_HTTP_V:
		if (recvd == '\r') {
			bigBuf[recCount] = '\0';
			recCount = 0;
			nextState = acceptHTTPVersion() ? REQ_LINE_END : HTTP_VERSION_NOT_SUPPORTED_505;
		}
		else 
			bigBuf[recCount++] = recvd;
		break;
	case REQ_LINE_END:
		nextState = (recvd == '\n') ? REQ_HEAD : BAD_REQ_400;
		break;

	case REQ_HEAD:
		if (recvd == '\r') {
#ifdef PLAB_DEBUG
			if (out)
				out->println("\nHeader line ending");
#endif // PLAB_DEBUG
			bigBuf[recCount] = '\0';
			recCount = 0;
			nextState = acceptHeader(true) ? REQ_HEAD_CR_ACC : INTERNAL_SERVER_ERROR_500;
		}
		else {
			bigBuf[recCount++] = recvd;
			// Special case, prevent buffer overflow. Should not result in 500, internal server error
			if (recCount == (PLAB_BIG_BUFFER_SIZE - 1)) {
				bigBuf[recCount] = '\0';
				recCount = 0;
				nextState = acceptHeader(false) ? REQ_HEAD : INTERNAL_SERVER_ERROR_500;
			}
		}
		break;
	case REQ_HEAD_CR_ACC:
#ifdef PLAB_DEBUG
		if (out)
			out->println("CR accepted");
#endif // PLAB_DEBUG
		nextState = (recvd == '\n') ? REQ_HEAD_LF_ACC : BAD_REQ_400;
		break;
	case REQ_HEAD_LF_ACC:
#ifdef PLAB_DEBUG
		if (out)
			out->println("Line feed has been accepted");
#endif // PLAB_DEBUG
		// Remember to cache received if this is not empty line
		if (recvd == '\r') {
			nextState = EMPTY_LINE;
		}
		else {
			nextState = REQ_HEAD;
			bigBuf[recCount++] = recvd;
		}
		break;

	case EMPTY_LINE:
#ifdef PLAB_DEBUG
		if (out)
			out->println("Accepting empty line");
#endif // PLAB_DEBUG
		nextState = (recvd == '\n') ? REQ_MESSAGE_BODY : BAD_REQ_400;
		break;

	case REQ_MESSAGE_BODY:
		// TODO callback to user to treat message body
		break;

	case BAD_REQ_400:
		break;
	case NOT_FOUND_404:
		break;
	case METHOD_NOT_ALLOWED_405:
		break;
	case REQUEST_URI_TOO_LONG_414:
		break;
	case INTERNAL_SERVER_ERROR_500:
		break;
	case NOT_IMPLEMENTED_501:
		break;
	case HTTP_VERSION_NOT_SUPPORTED_505:
		break;

	default:
		break;
	}

	// If we run out of buffer, this will be indicated by an internal server error
	if (recCount >= PLAB_BIG_BUFFER_SIZE) {
		nextState = INTERNAL_SERVER_ERROR_500;
	}
	
	return nextState;
}

// ---------------------------------------------------------------------------
// --- Main loop update ------------------------------------------------------
// ---------------------------------------------------------------------------
void PLabFileServer::update() {
	// Main loop.
	// Check if we have any new connections
	EthernetClient client = available();
	if (client) {
		if (out) {
			out->println();
			// Buffer is initially considered empty
			strcpy_P(bigBuf, plabRequest);
			out->println(bigBuf);
			out->println();
		}

		// We should now start parsing our request
		RequestState_t state = REQ_START;
		RequestMethod_t method = METH_NOT_SUPPORTED;
		int indexTracker = 0;
		// While we still have an open connection to the client
		while (client.connected()) {
			if (client.available()) {
				char c = client.read();
				if (out) {
					out->write(c);
				}
				state = reqStateTransition(state, c, indexTracker, method);
			}
			// By default we assume we have answered.
			bool answered = true;
			// Configuring answer, if we have any yet.
			switch (state) {
			case REQ_MESSAGE_BODY:
				if (out) {
					out->println();
					// Safe to overwrite buffer: it should not contain anything
					strcpy_P(bigBuf, plabResponse);
					out->println(bigBuf);
					out->println();
				}
				// GET and HEAD discard request body in this server
				// Currently this (HEAD and GET) covers every possibility.
				if (method == METH_HEAD || method == METH_GET) {
					// The exact same header
					strcpy_P(bigBuf, plabHttp200OK);
					// TODO CONTAIN THIS IN PROGMEM, eats up memory
					cPrintln(bigBuf, client);
					if (mimeTypeBuf[0] != '\0') {
						strcpy_P(bigBuf, plabContentType);
						cPrint(bigBuf, client);
						cPrintln(mimeTypeBuf, client);
					}
					strcpy_P(bigBuf, plabConnectionClose);
					cPrintln(bigBuf, client);
					// TODO Additional user defined header fields
					cPrintln("", client);	// HEADER stop
					if (method == METH_GET) {
#ifdef PLAB_DEBUG
						if (out)
							out->println("GET response");
#endif // PLAB_DEBUG
						// ONLY get has message body
						// TODO User defined body write
						if (sdFile) {
							while (sdFile.available()) {
								cWrite(sdFile.read(), client);
							}
							sdFile.close();
						}
#ifdef PLAB_DEBUG
						else if (out)
							out->println("File not open!");
#endif // PLAB_DEBUG
					}
				}
				break;

				// TODO Better error treatment
			case BAD_REQ_400:
				printDefaultError("400", "Bad Request", client);
				break;
			case NOT_FOUND_404:
				printDefaultError("404", "Not Found", client);
				break;
			case METHOD_NOT_ALLOWED_405:
				printDefaultError("405", "Method Not Allowed", client);
				break;
			case REQUEST_URI_TOO_LONG_414:
				printDefaultError("414", "Request-URI Too Large", client);
				break;
			case INTERNAL_SERVER_ERROR_500:
				printDefaultError("500", "Internal Server Error", client);
				break;
			case NOT_IMPLEMENTED_501:
				printDefaultError("501", "Not Implemented", client);
				break;
			case HTTP_VERSION_NOT_SUPPORTED_505:
				printDefaultError("505", "HTTP Version not supported", client);
				break;

			default:
				// No other state (as of yet) produces an answer
				answered = false;
			}

			// Clean, if need be
			if (answered) {
				if (sdFile) {
					sdFile.close();
				}

				// Client should have some time to process answer
				delay(1);

				client.stop();
				client.flush();
			}
		}
	}
}

void PLabFileServer::printDefaultError(const char* code, const char *reasonPhrase, EthernetClient &client) {
	if (out) {
		out->println();
		strcpy_P(bigBuf, plabResponse);
		out->println(bigBuf);
		out->println();
	}
	strcpy_P(bigBuf, plab400BadRequest);
	// Use layout of message, divide manually
	bigBuf[3] = '\0';
	printDefaultError(bigBuf, &(bigBuf[4]), client);
	cPrint("HTTP/1.1 ", client);
	cPrint(code, client);
	cPrint(" ", client);
	cPrintln(reasonPhrase, client);
	//cPrintln("Content-Type: text/html", client);
	cPrintln("Connection: close", client);
	cPrintln("", client);
	/*cPrint("<html><head><title>", client);
	cPrint(code, client);
	cPrint(" - ", client);
	cPrint(reasonPhrase, client);
	cPrint("</title></head><body><h1>", client);
	cPrint(code, client);
	cPrint(": ", client);
	cPrint(reasonPhrase, client);
	cPrintln("</h1></body></html>", client);*/
}

bool PLabFileServer::addMIMEType(const char *suffix, const char *mediaType) { return false; }
