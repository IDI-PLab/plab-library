/*
* Programming Lab Code URI Protocol
* Version 0.1 August, 2015
*
* Arduino side of PLCUP.
*
* Author Inge Edward Halsaunet
* released into the public domain
*/

#ifndef PLCUP_H
#define PLCUP_H

#include <Stream.h>

class AbstractPLabCodeSource {
public:
	virtual int available() = 0;
	virtual char read() = 0;
};

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

#endif