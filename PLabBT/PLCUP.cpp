/*
class PLCUP {
private:
enum state_t {
ACC_START, ACC_VER, SEND_CAP, ACC_GET_START, ACC_GET, ACC_GET_CODE, ACC_GET_URI, SEND_CODE, SEND_URI, SEND_ERR
};

state_t state = ACC_START;
unsigned int cPos = 0;

char *_codeLoc = NULL;
AbstractPLabCodeSource *_codeSource = NULL;

const static char PLCUP_s[];
const static char VERSION_s[];
const static char CODE_s[];
const static char URI_s[];
const static char NONE_s[];
const static char CODE_URI_s[];
const static char ERROR_s[];
const static char BEGIN_s[];
const static char END_s[];
const static char GET_s[];

unsigned long timeout = 60000; // Defaults to 60 seconds
unsigned long startTime;

void sendCapabilities(Stream &to);
void sendCode(Stream &to);
void sendURI(Stream &to);
void sendBlobBeginToken(Stream &to);
void sendBlobEndToken(Stream &to);
void sendError(Stream &to);
public:
~PLCUP() { delete[] _codeLoc; }

void setPLCUPCodeURI(const char *loc);
void setPLCUPCodeSourcePointer(AbstractPLabCodeSource *source);

bool processPLCUPCharacter(char c);	// true if PLCUP should take control of out stream
void replyPLCUP(Stream &to);

bool isPLCUPWorking();
void setPLCUPTimeout(unsigned int milliseconds) { timeout = milliseconds; }
};
*/

#include "Arduino.h"
#include "PLCUP.h"

const char PLCUP::PLCUP_s[] = "PLCUP";
const char PLCUP::VERSION_s[] = "/0.1";
const char PLCUP::CODE_s[] = "COD";
const char PLCUP::URI_s[] = "URI";
const char PLCUP::NONE_s[] = "NON";
const char PLCUP::CODE_URI_s[] = "C/U";
const char PLCUP::ERROR_s[] = "ERR";
const char PLCUP::BEGIN_s[] = "BEG";
const char PLCUP::END_s[] = "END";
const char PLCUP::GET_s[] = " G ";

void PLCUP::sendCapabilities(Stream &to) {
	to.print(PLCUP_s);
	to.print(" ");
	if (_codeLoc) {
		if (_codeSource) {
			to.println(CODE_URI_s);
		}
		else {
			to.println(URI_s);
		}
		state = ACC_GET_START;
	}
	else if (_codeSource) {
		to.println(CODE_s);
		state = ACC_GET_START;
	}
	else {
		to.println(NONE_s);
		state = ACC_START;
	}
}


void PLCUP::sendCode(Stream &to) {
	sendBlobBeginToken(to);
	if (_codeSource) {
		while (_codeSource->available() > 0)
			to.write(_codeSource->read());
	}
	to.println();
	sendBlobEndToken(to);
}


void PLCUP::sendURI(Stream &to) {
	sendBlobBeginToken(to);
	if (_codeLoc)
		to.print(_codeLoc);
	to.println();
	sendBlobEndToken(to);
}


void PLCUP::sendBlobBeginToken(Stream &to) {
	to.print(PLCUP_s);
	to.print(" ");
	to.println(BEGIN_s);
}


void PLCUP::sendBlobEndToken(Stream &to) {
	to.print(PLCUP_s);
	to.print(" ");
	to.println(END_s);
	to.println();
	state = ACC_START;
}


void PLCUP::sendError(Stream &to) {
	to.print(PLCUP_s);
	to.print(" ");
	to.println(ERROR_s);
	state = ACC_START;
}


void PLCUP::setPLCUPCodeURI(const char *loc) {
	if (_codeLoc)
		delete[] _codeLoc;
	int len = strlen(loc);
	_codeLoc = new char[len + 1];	// length + '\0'
	strcpy(_codeLoc, loc);
}

void PLCUP::setPLCUPCodeSourcePointer(AbstractPLabCodeSource *source) {
	_codeSource = source;
}

bool PLCUP::processPLCUPCharacter(char c) {	// true if PLCUP should take control of out stream
	// Check if a timeout has occured
	if (state != ACC_START && (millis() - startTime) > timeout) {
		state = SEND_ERR;
	}
	else {
		// Restart timer
		startTime = millis();

		switch (state) {
		case ACC_START:
			if (PLCUP_s[cPos++] != c) {
				cPos = 0;
			}
			if (PLCUP_s[cPos] == '\0') {
				cPos = 0;
				state = ACC_VER;
			}
			break;
		case ACC_VER:
			if (VERSION_s[cPos++] != c) {
				cPos = 0;
				state = SEND_ERR;
			}
			if (VERSION_s[cPos] == '\0') {
				cPos = 0;
				state = SEND_CAP;
			}
			break;
		case SEND_CAP:
			if (PLCUP_s[0] == c) {
				cPos = 1;
				if (_codeLoc || _codeSource) {
					state = ACC_GET_START;
				}
				else {
					state = ACC_START;
				}
			}
			else {
				cPos = 0;
				if (_codeLoc || _codeSource) {
					state = SEND_ERR;
				}
				else {
					state = ACC_START;
				}
			}
			break;
		case ACC_GET_START:
			if (PLCUP_s[cPos++] != c) {
				cPos = 0;
				state = SEND_ERR;
			}
			if (PLCUP_s[cPos] == '\0') {
				cPos = 0;
				state = ACC_GET;
			}
			break;
		case ACC_GET:
			if (GET_s[cPos] == '\0') {
				switch (c) {
				case 'C':	// CODE_s[0]
					cPos = 1;
					state = ACC_GET_CODE;
					break;

				case 'U':	// URI_s[0]
					cPos = 1;
					state = ACC_GET_URI;
					break;
				default:
					cPos = 0;
					state = SEND_ERR;
					break;
				}
			}
			else if (GET_s[cPos++] != c) {
				cPos = 0;
				state = SEND_ERR;
			}
			break;
		case ACC_GET_CODE:
			if (CODE_s[cPos++] != c) {
				cPos = 0;
				state = SEND_ERR;
			}
			if (CODE_s[cPos] == '\0') {
				cPos = 0;
				state = SEND_CODE;
			}
			break;
		case ACC_GET_URI:
			if (URI_s[cPos++] != c) {
				cPos = 0;
				state = SEND_ERR;
			}
			if (URI_s[cPos] == '\0') {
				cPos = 0;
				state = SEND_URI;
			}
			break;
		case SEND_CODE:
			// Fallthrough
		case SEND_URI:
			// Fallthrough
		case SEND_ERR:
			// Fallthrough
		default:
			cPos = c == PLCUP_s[0] ? 1 : 0;
			state = ACC_START;
			break;
		}
	}

	return state == SEND_CAP || state == SEND_CODE || state == SEND_URI || state == SEND_ERR;
}
void PLCUP::replyPLCUP(Stream &to) {
	switch (state) {
	case SEND_CAP:
		sendCapabilities(to);
		break;
	case SEND_CODE:
		sendCode(to);
		break;
	case SEND_URI:
		sendURI(to);
		break;
	case SEND_ERR:
		sendError(to);
		break;
	default:
		;
	}
}

bool PLCUP::isPLCUPWorking() {
	return !(state == ACC_START || state == SEND_CODE || state == SEND_URI || state == SEND_ERR || (state == SEND_CAP && !(_codeLoc || _codeSource)));
}