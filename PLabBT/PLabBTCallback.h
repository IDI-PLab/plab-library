#ifndef PLAB_BT_CALLBACK_H
#define PLAB_BT_CALLBACK_H

#include <inttypes.h>
#include <PLabBTSerial.h>

struct FuncListStruct {
public:
	FuncListStruct *next;
	void(*callback)(char *arg);
	char *command;

	FuncListStruct()
	{
		next = 0;
		command = 0;
	}
	FuncListStruct(char *c, void(*cb)(char *arg), FuncListStruct *n)
	{
		next = n;
		command = c;
		callback = cb;
	}
	~FuncListStruct()
	{
		delete next;
	}
};

class PLabBTCallback
{
private:
	FuncListStruct *funcLists;
	PLabBTSerial transiever;
	char cmdDivider;
	void(*emptyCallback)(char* arg);
	const uint8_t bSize;
public:
	PLabBTCallback(uint8_t receivePin, uint8_t transmitPin, char divider = ':', bool inverse_logic = false, uint8_t bufferSize = 50);
	~PLabBTCallback();

	void begin(long speed);

	void update();

	void registerCallback(char *command, void(*callback)(char *arg));
	void registerNoCommandCallback(void(*callback)(char *arg));
};

#endif

