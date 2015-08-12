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
	virtual int available() = delete;
	virtual char read() = delete;
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

	const static char PLCUP_s[] = "PLCUP";
	const static char VERSION_s[] = "/0.1";
	const static char CODE_s[] = "COD";
	const static char URI_s[] = "URI";
	const static char NONE_s[] = "NON";
	const static char CODE_URI_s[] = "C/U";
	const static char ERROR_s[] = "ERR";
	const static char BEGIN_s[] = "BEG";
	const static char END_s[] = "END";
	const static char GET_s[] = " G ";

	void sendCapabilities(Stream &to);
	void sendCode(Stream &to);
	void sendURI(Stream &to);
	void sendBlobBeginToken(Stream &to);
	void sendBlobEndToken(Stream &to);
	void sendError(Stream &to);
public:
	~PLCUP() { delete[] _codeLoc; }

	void setCodeURI(const char *loc);
	void setCodeSourcePointer(AbstractPLabCodeSource *source);

	bool processCharacter(char c);	// true if PLCUP should take control of out stream
	void reply(Stream &to);
};

#endif