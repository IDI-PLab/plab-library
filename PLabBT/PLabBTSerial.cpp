#include <PLabBTSerial.h>

int PLabBTSerial::read() {
	if (_available)
	{
		int r = _buffer[_retLoc];
		_available--;
		if (_available)
		{
			_retLoc++;
		}
		else
		{
			_bLoc = 0;
			_retLoc = 0;
		}
		return r;
	}
	return -1;
}

void PLabBTSerial::read(char* buffer, int bufferSize) {
	if (_available > bufferSize)
	{
		return;
	}
	int i = 0;
	while (_available)
	{
		buffer[i] = read();
		i++;
	}
}

int PLabBTSerial::available()
{
	if (_available > 0)
	{
		return _available;
	}
	while (SoftwareSerial::available() > 0)
	{
		_buffer[_bLoc] = SoftwareSerial::read();
		if (_buffer[_bLoc] == '\r')
		{
			// Should just skip 
		}
		else if (_buffer[_bLoc] == '\n')
		{
			// New line found. End string and return available data count
			_buffer[_bLoc] = '\0';
			_available = (_bLoc - _retLoc) + 1;
			return _available;
		}
		else
		{
			_bLoc++;
		}
		while (_bLoc >= _bufferSize)
		{
			_bLoc--;
		}
	}
	return 0;
}

// Constructor
PLabBTSerial::PLabBTSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic /*= false*/, uint8_t bufferSize /*= 50*/) :
		SoftwareSerial(receivePin, transmitPin, inverse_logic)
{
	_bufferSize = bufferSize;
	_buffer = new char[_bufferSize];
	_bLoc = 0;
	_retLoc = 0;
	_available = 0;
}

// Destructor
PLabBTSerial::~PLabBTSerial()
{
	delete[] _buffer;
	end();
}


