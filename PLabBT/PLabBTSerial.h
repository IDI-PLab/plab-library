
/*
 * PLabBTSerial
 * Version 0.1 February, 2015
 * Library that simplify serial line communication using SoftwareSerial.
 * Allows the user to batch up characters to form an entire line before reading
 *
 * Author Inge Edward Halsaunet
 */


#ifndef PLAB_BT_SERIAL_H
#define PLAB_BT_SERIAL_H

#include <SoftwareSerial.h>
#include <inttypes.h>

class PLabBTSerial : public SoftwareSerial
{
private:
        // Size of buffer.
	uint8_t _bufferSize;
	// how far is written into the buffer
	uint8_t _bLoc;
	// How much is read from user
	uint8_t _retLoc;
	// How much is left available to read on this line
	int _available;
	// the buffer
	char *_buffer;
public:
	PLabBTSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false, uint8_t bufferSize = 50);
	~PLabBTSerial();

        // The ones that will be overriden
	virtual int read();
	virtual int available();
	
	// Completely new method. Fills the given buffer with data received,
	// given the buffer is large enough. Given buffer size identified as
	// bufferSize
	virtual void read(char* buffer, int bufferSize);
	
};

#endif
