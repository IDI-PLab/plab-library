
/*
 * PLabBTSerial
 * Version 0.1 February, 2015
 * Library that simplify serial line communication using SoftwareSerial.
 * Allows the user to batch up characters to form an entire line before reading
 *
 * Author Inge Edward Halsaunet
 * released into the public domain
 */

//#define PLAB_DEBUG


#ifndef PLAB_BT_SERIAL_H
#define PLAB_BT_SERIAL_H

#include <SoftwareSerial.h>
#include <inttypes.h>
#include "PLCUP.h"

#ifdef PLAB_DEBUG
#include "Arduino.h"
#endif	// PLAB_DEBUG

class PLabBTSerial : public SoftwareSerial, private PLCUP
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

#ifdef PLAB_DEBUG
	size_t write(uint8_t ch) {
		Serial.print("w:");
		Serial.write(ch);
		Serial.print(" ");
		return SoftwareSerial::write(ch);
	}
#endif	// PLAB_DEBUG

	// Those method inherited from PLCUP that should be made public
	using PLCUP::setPLCUPTimeout;
	using PLCUP::setPLCUPCodeURI;
	using PLCUP::setPLCUPCodeSourcePointer;
	
};

#endif
