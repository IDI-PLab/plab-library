#include <PLabBTCallback.h>
#include <string.h>


PLabBTCallback::PLabBTCallback(uint8_t receivePin, uint8_t transmitPin, char divider, bool inverse_logic, uint8_t bufferSize) :
		PLabBTSerial(receivePin, transmitPin, inverse_logic, bufferSize), bSize (bufferSize)
{
	cmdDivider = divider;
	funcLists = 0;
	emptyCallback = 0;
}


PLabBTCallback::~PLabBTCallback()
{
	delete funcLists;
}

void PLabBTCallback::update()
{
	int availableCount = available();
	if (availableCount > 0)
	{
		char *msg = new char[availableCount];
		read(msg, availableCount);
		char *divided = strchr(msg, cmdDivider);
		if (divided == 0 && emptyCallback != 0)
		{
			emptyCallback(msg);
		}
		else
		{
			divided[0] = 0;
			divided++;
			FuncListStruct *fp = funcLists;
			while (fp != 0)
			{
				if (strcmp(fp->command, msg) == 0) {
					// found callback! All callbacks registered to this command will be run
					fp->callback(divided);
				}
				fp = fp->next;
			}
		}

		delete[] msg;
	}
}

void PLabBTCallback::registerCallback(char *command, void(*callback)(char *arg))
{
	// Count command length and copy.
	int l = 0;
	while (command[l] != 0) { l++; }
	l++;
	char *cmd = new char[l];
	for (int i = 0; i < l; i++) { cmd[i] = command[i]; }
	// Add function to list of functions
	funcLists = new FuncListStruct(cmd, callback, funcLists);
}
void PLabBTCallback::registerNoCommandCallback(void(*callback)(char *arg))
{
	emptyCallback = callback;
}
