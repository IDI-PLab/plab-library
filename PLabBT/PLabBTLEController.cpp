#include "PLabBTLEController.h"


void PLabBTLEController::init(long speed)
{
	SoftwareSerial::begin(speed);
	name = 0;
	role = UNDEFINED;
	_devices = new PLabBTLEDevice[PLAB_BTLE_MAXDEVICE];
	// Give the connection some time to set up
	delay(PLAB_BTLE_START_PAUSE);
	state = INIT;
}
const PLabBTLEDevice &PLabBTLEController::operator[](const int &index) const
{
	return _devices[index];
}

void PLabBTLEController::updateName()
{
	delete[] name;
#ifdef PLAB_DEBUG
	Serial.println(engine.getMessage());
#endif
	int size = engine.update(0);
	name = new char[size + 1];
	for (int i = 0; i <= size; i++)
	{
		name[i] = engine.getMessage()[i];
	}
#ifdef PLAB_DEBUG
	Serial.println(size);
	Serial.println(name);
#endif
}

void PLabBTLEController::update()
{
#ifdef PLAB_DEBUG
	static State pState = state;
	if (state != pState) {
		Serial.println(state);
		pState = state;
	}
#endif
	// READY should not handle anything
	if (state == READY) return;

	switch (state)
	{
	case INIT:
		state = INIT_GETNAME;
		engine.prefix = statePrefixes[statePrefixIndex[state]];
		commandIssued = millis();
		replyStarted = false;
		write("AT+NAME?");
		break;
	case INIT_GETNAME:
		if (SoftwareSerial::available())
		{
			replyStarted = true;
			int c = read();
			if (c == 0) {
				if (engine.prefixRead()) {
					updateName();
					state = INIT_GETROLE;
					engine.prefix = statePrefixes[statePrefixIndex[state]];
					commandIssued = millis();
					replyStarted = false;
					write("AT+ROLE?");
				}
			} else {
				engine.update(c);
			}
		}
		else if (!replyStarted && ((millis() - commandIssued) > PLAB_TIMEOUT))
		{
			commandIssued = millis();
			write("AT+NAME?");
		}
		break;
	case INIT_GETROLE:
		if (SoftwareSerial::available())
		{
			replyStarted = true;
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				if (r == '0') role = PERIPHERAL;
				else if (r == '1') role = CENTRAL;
				else role = UNDEFINED;
				state = INIT_GETDISCNAME;
				commandIssued = millis();
				replyStarted = false;
				write("AT+SHOW?");
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
		}
		else if (!replyStarted && ((millis() - commandIssued) > PLAB_TIMEOUT))
		{
			commandIssued = millis();
			write("AT+ROLE?");
		}
		break;
	case INIT_GETDISCNAME:
		if (SoftwareSerial::available())
		{
			replyStarted = true;
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				discoverNames = r == '1';
				state = INIT_WORKIMM;
				commandIssued = millis();
				replyStarted = false;
				write("AT+IMME?");
				engine.prefix = statePrefixes[statePrefixIndex[state]];

			}
		}
		else if (!replyStarted && ((millis() - commandIssued) > PLAB_TIMEOUT))
		{
			commandIssued = millis();
			write("AT+SHOW?");
		}
		break;
	case INIT_WORKIMM:
		if (SoftwareSerial::available())
		{
			replyStarted = true;
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				workImmediately = r == '0';
				state = READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
				replyStarted = false;
			}
		}
		else if (!replyStarted && ((millis() - commandIssued) > PLAB_TIMEOUT))
		{
			commandIssued = millis();
			write("AT+IMME?");
		}
		break;
	case SETTING_ROLE:
		if (SoftwareSerial::available())
		{
			replyStarted = true;
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				if (r == '0') role = PERIPHERAL;
				else if (r == '1') role = CENTRAL;
				else role = UNDEFINED;
				state = READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
				replyStarted = false;
			}
		}
		else if (!replyStarted && ((millis() - commandIssued) > PLAB_TIMEOUT))
		{
			commandIssued = millis();
			engine.prefix = statePrefixes[statePrefixIndex[INIT_GETROLE]];
			write("AT+ROLE?");
		}
		break;
	case SETTING_NAME:
		if (SoftwareSerial::available() || nameLength == nameRcvd) {
			replyStarted = true;
			int c = read();
			if (c == 0 || nameLength == nameRcvd)
			{
				updateName();
				state = READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
				replyStarted = false;
			}
			else
			{
				if (engine.update(c) > 0) { ++nameRcvd; }
			}
		}
		else if (!replyStarted && ((millis() - commandIssued) > PLAB_TIMEOUT))
		{
			commandIssued = millis();
			engine.prefix = statePrefixes[statePrefixIndex[INIT_GETNAME]];
			write("AT+NAME?");
		}
		break;
	case SETTING_NAME_DISCOVERY:
		if (SoftwareSerial::available())
		{
			replyStarted = true;
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				discoverNames = r == '1';
				state = READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
				replyStarted = false;
			}
		}
		else if (!replyStarted && ((millis() - commandIssued) > PLAB_TIMEOUT))
		{
			commandIssued = millis();
			engine.prefix = statePrefixes[statePrefixIndex[INIT_GETDISCNAME]];
			write("AT+SHOW?");
		}
		break;
	case SETTING_WORKIMM:
		if (SoftwareSerial::available())
		{
			replyStarted =  true;
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				workImmediately = r == '0';
				state = READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
				replyStarted = false;
			}
		}
		else if (!replyStarted && ((millis() - commandIssued) > PLAB_TIMEOUT))
		{
			commandIssued = millis();
			engine.prefix = statePrefixes[statePrefixIndex[INIT_WORKIMM]];
			write("AT+IMME?");
		}
		break;
	case DISCOVERY_STARTING:
		if (SoftwareSerial::available())
		{
			replyStarted = true;
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
#ifdef PLAB_DEBUG
				Serial.print("Discovery has started, going to next stage if 'S'==");
				Serial.println(r);
#endif
				state = r == 'S' ? DISCOVERY_RUNNING : READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
		}
		else if (!replyStarted && ((millis() - commandIssued) > PLAB_TIMEOUT))
		{
			commandIssued = millis();
			write("AT+DISC?");
		}
		break;
	case DISCOVERY_RUNNING:
		if (SoftwareSerial::available())
		{
			int c = read();
			if (engine.update(c) > 0)
			{
				char r = engine.getMessage()[0];
				// Assuming non-hex numbering of devices
				if (r == 'C')
				{
					state = DISCOVERY_ENDING;
					engine.prefix = statePrefixes[statePrefixIndex[state]];
					engine.update(0);
					engine.update(r);
#ifdef PLAB_DEBUG
					Serial.println("Going to discovery ending");
#endif
				}
				else
				{
					// Assuming less than 10 discoverable devices
					discovering = r - '0';
					discovering = discovering < PLAB_BTLE_MAXDEVICE ? discovering : -1;
					if (discovering >= 0)
					{
						_devices[discovering].id = discovering;
						delete[] (_devices[discovering].name);
						_devices[discovering].name = 0;
					}
					engine.update(0);
					state = DISCOVERY_ADDRESS;
					engine.prefix = statePrefixes[statePrefixIndex[state]];
				}
			}
		}
		else if ((millis() - commandIssued) > PLAB_LONG_TIMEOUT)
		{
#ifdef PLAB_DEBUG
			Serial.println("Discovery software time-out");
#endif
			replyStarted = false;
			engine.update(0);
			state = READY;
			engine.prefix = statePrefixes[statePrefixIndex[state]];
		}
		break;
	case DISCOVERY_ENDING:
		if (SoftwareSerial::available())
		{
			int c = read();
			if (engine.update(c) > 0) // && c == 'E')
			{
				// This should be an 'E', but since we do not have any fail state...
				state = READY;
				engine.update(0);
				engine.prefix = statePrefixes[statePrefixIndex[state]];
				replyStarted = false;
			}
		}
		break;
	case DISCOVERY_NAME:
		if (SoftwareSerial::available())
		{
			int c = read();
			// Documentation says we will receive \r\n at the end of name
			if (c == '\n')
			{
#ifdef PLAB_DEBUG
				Serial.println("Going to discovery running");
#endif
				state = DISCOVERY_RUNNING;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
			else if (c == '\r')
			{
				int nameChars = engine.update(0);
#ifdef PLAB_DEBUG
				Serial.print("End of name found, name length: ");
				Serial.println(nameChars);
#endif
				if (discovering >= 0) {
					_devices[discovering].name = new char[nameChars + 1];
					for (int i = 0; i <= nameChars; i++)
						_devices[discovering].name[i] = engine.getMessage()[i];
				}
			}
			else if (c != 0)
			{
				engine.update(c);
			}
		}
		break;
	case DISCOVERY_ADDRESS:
		if (SoftwareSerial::available())
		{
			
			int c = read();
			// Ending on receiving start of next message
			if (c == 'O')
			{
				int size = engine.update(0);
				if (discovering >= 0)
				{
					delete[] (_devices[discovering].address);
					_devices[discovering].address = new char[size + 1];
					for (int i = 0; i <= size; ++i)
						_devices[discovering].address[i] = engine.getMessage()[i];
				}
#ifdef PLAB_DEBUG
				if (discoverNames) Serial.println("Going to discover name");
				else Serial.println("Going to discovery running");
#endif
				state = discoverNames ? DISCOVERY_NAME : DISCOVERY_RUNNING;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
				engine.update(c);
			}
			else
			{
				engine.update(c);
			}
		}
		break;
	case CONNECT_RUNNING:
		if (SoftwareSerial::available())
		{
			replyStarted = true;
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				// Normal connect:
				// Valid: 0-5. Invalid: E/F
				// Connect last:
				// Valid: L, Invalid: E/F/N
				// Connect with address:
				// Valid: A, Invalid: E/F
				connected = (r >= '0' && r <= '5') || r == 'L' || r == 'A';
				connFail = r == 'E' || r == 'F' || r == 'N';
				replyStarted = false;
				state = READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
		}
		else if (!replyStarted && ((millis() - commandIssued) > PLAB_TIMEOUT))
		{
			connFail = true;
			state = READY;
			engine.prefix = statePrefixes[statePrefixIndex[state]];
		}
		break;
	case DISCONNECTING:
		connected = workImmediately;
		state = READY;
		break;
	case RESET_IN_PROGRESS:
		if (SoftwareSerial::available())
		{
			replyStarted = true;
			if (engine.update(read()) > 0 || engine.prefixRead())
			{
				replyStarted = false;
				state = READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
		}
		else if (!replyStarted && ((millis() - commandIssued) > PLAB_TIMEOUT))
		{
			commandIssued = millis();
			write("AT+RESET");
		}
		break;
	case FACTORY_RESET_IN_PROGRESS:
		if (SoftwareSerial::available())
		{
			replyStarted = true;
			if (engine.update(read()) > 0 || engine.prefixRead())
			{
				replyStarted = false;
				engine.update(0);
				delay(PLAB_FACT_RESET_PAUSE);
				state = INIT;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
		}
		else if (!replyStarted && ((millis() - commandIssued) > PLAB_TIMEOUT))
		{
			commandIssued = millis();
			write("AT+RENEW");
		}
		break;
	}
}
bool PLabBTLEController::isReady() const { return state == READY; }

void PLabBTLEController::setName(char *_name)
{
	if (!isReady() || connected) return;
	state = SETTING_NAME;
	delete[] name;
	name = 0;
	nameLength = 0;
	nameRcvd = 0;
	while (_name[nameLength] != 0) { ++nameLength; }
	commandIssued = millis();
	replyStarted = false;
	print("AT+NAME");
	print(_name);
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}
const char* PLabBTLEController::getName() const
{
	if (state >= READY && state != SETTING_NAME)
	{
		return name;
	}
	return 0;
}

PLabBTLEController::Role PLabBTLEController::getRole() const { return role; }
void PLabBTLEController::setRole(Role r) {
	if (!isReady() || r == UNDEFINED || connected) return;
	state = SETTING_ROLE;
	commandIssued = millis();
	replyStarted = false;
	print("AT+ROLE");
	print((char)r);
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}
bool PLabBTLEController::isPeripheral() const
{
	return role == PERIPHERAL;
}
bool PLabBTLEController::isCentral() const
{
	return role == CENTRAL;
}

bool PLabBTLEController::isDiscoveringNames() const { return discoverNames; }
void PLabBTLEController::setDiscoveringNames(bool disc)
{
	if (!isReady() || connected) return;
	state = SETTING_NAME_DISCOVERY;
	commandIssued = millis();
	replyStarted = false;
	if (disc)
		write("AT+SHOW1");
	else
		write("AT+SHOW0");
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}

bool PLabBTLEController::isWorkingImmediately() const { return workImmediately; }
void PLabBTLEController::setWorkingImmediately(bool wi)
{
	if (!isReady() || connected) return;
	state = SETTING_WORKIMM;
	commandIssued = millis();
	replyStarted = false;
	if (wi)
		write("AT+IMME0");
	else
		write("AT+IMME1");
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}

void PLabBTLEController::discoverDevices()
{
	if (!isReady() || connected || role != CENTRAL || workImmediately) return;
	state = DISCOVERY_STARTING;
	engine.prefix = statePrefixes[statePrefixIndex[state]];
	clearDiscoveredDevices();
	commandIssued = millis();
	replyStarted = false;
	write("AT+DISC?");
}
bool PLabBTLEController::isDiscoveringDevices() const { return state == DISCOVERY_RUNNING; }
int PLabBTLEController::devices() const
{
	for (int i = 0; i < PLAB_BTLE_MAXDEVICE; ++i)
	{
		if (_devices[i].id < 0) return i;
	}
	return PLAB_BTLE_MAXDEVICE;
}
void PLabBTLEController::clearDiscoveredDevices()
{
	for (int i = 0; i < PLAB_BTLE_MAXDEVICE; ++i)
	{
		_devices[i].id = -1;
		delete[] _devices[i].name;
		_devices[i].name = 0;
		delete[] _devices[i].address;
		_devices[i].address = 0;
	}
}

void PLabBTLEController::connectDevice(int id)
{
	if (!isReady() || connected || role != CENTRAL || workImmediately) return;
	if (id >= devices()) return;
	state = CONNECT_RUNNING;
	commandIssued = millis();
	replyStarted = false;
	print("AT+CONN");
	print(id);
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}
void PLabBTLEController::connectDevice(const PLabBTLEDevice &device)
{
	if (!isReady() || connected || role != CENTRAL || workImmediately || (device.id < 0 && device.address == 0)) return;
	if (device.address != 0)
	{
		connectDevice(device.address);
	}
	else
	{
		state = CONNECT_RUNNING;
		commandIssued = millis();
		replyStarted = false;
		print("AT+CONN");
		print(device.id);
		engine.prefix = statePrefixes[statePrefixIndex[state]];
	}
}
void PLabBTLEController::connectDevice(const char *address)
{
	if (!isReady() || connected || role != CENTRAL || workImmediately || address == 0) return;
	state = CONNECT_RUNNING;
	commandIssued = millis();
	replyStarted = false;
	print("AT+CON");
	print(address);
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}
void PLabBTLEController::connectLastDevice()
{
	if (!isReady() || connected || role != CENTRAL || workImmediately) return;
	state = CONNECT_RUNNING;
	commandIssued = millis();
	replyStarted = false;
	write("AT+CONNL");
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}
bool PLabBTLEController::connectionFailed() const { return connFail; }
bool PLabBTLEController::isConnecting() const { return state == CONNECT_RUNNING; }
bool PLabBTLEController::isConnected() const { return connected; }
void PLabBTLEController::disconnect()
{
	if (!isReady() || !connected || role != CENTRAL) return;
	state = DISCONNECTING;
	commandIssued = millis();
	replyStarted = false;
	write("AT");
	connected = false;
	connFail = false;
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}

void PLabBTLEController::reset()
{
	if (!isReady() || connected) return;
	state = RESET_IN_PROGRESS;
	commandIssued = millis();
	replyStarted = false;
	write("AT+RESET");
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}
void PLabBTLEController::factoryReset()
{
	if (!isReady() || connected) return;
	state = FACTORY_RESET_IN_PROGRESS;
	commandIssued = millis();
	replyStarted = false;
	write("AT+RENEW");
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}


// STATE ENGINE implementation
int PLabBTLEController::PLabBTLEStateEngine::update(char c) {
	switch (state)
	{
	case START:
		bLoc = 0;
		buffer[bLoc] = 0;
		if (prefix == 0)
		{
			state = RECV_MSG;
		}
		else
		{
			pLoc = 0;
			state = RECV_PREFIX;
			return prefix[pLoc++] == c ? 0 : -1;
		}
		break;
	case RECV_PREFIX:
		if (prefix[pLoc] == 0)
		{
			state = RECV_MSG;
		}
		else
		{
			return prefix[pLoc++] == c ? 0 : -1;
		}
		break;
	}
	// If we get here, we are in state RECV_MSG
	if (c == 0)
	{
		state = START;
		return bLoc;
	}
	if (bLoc >= (PLAB_BTLE_BSIZE - 1))
	{
		buffer[PLAB_BTLE_BSIZE - 1] = 0;
		return -1;
	}
	buffer[bLoc++] = c;
	buffer[bLoc] = 0;
	return 1;
}
char *PLabBTLEController::PLabBTLEStateEngine::getMessage() { return buffer; }
bool PLabBTLEController::PLabBTLEStateEngine::prefixRead()
{
	return prefix == 0 || prefix[pLoc] == 0;
}

#ifdef PLAB_DEBUG
void PLabBTLEController::PLabBTLEStateEngine::printState() {
	Serial.print("Engine state: ");
	switch (state)
	{
	case START:
		Serial.println("Start");
		break;
	case RECV_PREFIX:
		Serial.println("Rcv prefix");
		break;
	case RECV_MSG:
		Serial.println("Rcv msg");
		break;
	}
}
#endif
