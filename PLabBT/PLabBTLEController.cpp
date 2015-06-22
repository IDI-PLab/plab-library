#define PLAB_DEBUG
#include "PLabBTLEController.h"


void PLabBTLEController::init(long speed)
{
	SoftwareSerial::begin(speed);
	state = INIT;
	name = 0;
	role = UNDEFINED;
	_devices = new PLabBTLEDevice[PLAB_BTLE_MAXDEVICE];
}
const PLabBTLEDevice &PLabBTLEController::operator[](const int &index) const
{
	return _devices[index];
}

void PLabBTLEController::updateName()
{
	delete[] name;
	int size = engine.update(0);
	name = new char[size + 1];
	for (int i = 0; i <= size; i++)
	{
		name[i] = engine.getMessage()[i];
	}
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
	unsigned long now = millis();
	// If the current state is not dependent on overTime, update lastTime
	if (state == INIT ||
		state == INIT_GETROLE ||
		state == INIT_GETDISCNAME ||
		state == INIT_WORKIMM ||
		state == SETTING_ROLE ||
		state == SETTING_NAME_DISCOVERY ||
		state == SETTING_WORKIMM ||
		state == DISCOVERY_STARTING ||
		state == DISCOVERY_RUNNING ||
		state == DISCOVERY_ENDING ||
		state == DISCOVERY_NAME ||
		state == CONNECT_RUNNING ||
		state == DISCONNECTING)
	{
		lastTime = now;
	}

	// READY should not handle anything but lastTime update
	if (state == READY) return;

	bool overTime = (now - lastTime) >= PLAB_BTLE_AT_PAUSE;

	switch (state)
	{
	case INIT:
		state = INIT_GETNAME;
		write("AT+NAME?");
		engine.prefix = statePrefixes[statePrefixIndex[state]];
		break;
	case INIT_GETNAME:
		if (overTime)
		{
			updateName();
			state = INIT_GETROLE;
			write("AT+ROLE?");
			engine.prefix = statePrefixes[statePrefixIndex[state]];
		}
		else if (SoftwareSerial::available())
			engine.update(read());
		break;
	case INIT_GETROLE:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				if (r == '0') role = PERIPHERAL;
				else if (r == '1') role = CENTRAL;
				else role = UNDEFINED;
				state = INIT_GETDISCNAME;
				write("AT+SHOW?");
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
		}
		break;
	case INIT_GETDISCNAME:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				discoverNames = r == '1';
				state = INIT_WORKIMM;
				write("AT+IMME?");
				engine.prefix = statePrefixes[statePrefixIndex[state]];

			}
		}
		break;
	case INIT_WORKIMM:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				workImmediately = r == '0';
				state = READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];

			}
		}
		break;
	case SETTING_ROLE:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				if (r == '0') role = PERIPHERAL;
				else if (r == '1') role = CENTRAL;
				else role = UNDEFINED;
				state = READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
		}
		break;
	case SETTING_NAME:
		if (overTime)
		{
			updateName();
			state = READY;
			engine.prefix = statePrefixes[statePrefixIndex[state]];
		}
		else if (SoftwareSerial::available())
			engine.update(read());
		break;
	case SETTING_NAME_DISCOVERY:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				discoverNames = r == '1';
				state = READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];

			}
		}
		break;
	case SETTING_WORKIMM:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				workImmediately = r == '0';
				state = READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
		}
		break;
	case DISCOVERY_STARTING:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				state = r == 'S' ? DISCOVERY_RUNNING : READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
		}
		break;
	case DISCOVERY_RUNNING:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				// Assuming non-hex numbering of devices
				if (r == 'C')
				{
					state = DISCOVERY_ENDING;
					engine.prefix = statePrefixes[statePrefixIndex[state]];
					engine.update(r);
				}
				else
				{
					// Assuming less than 10 discoverable devices
					discovering = r - '0';
					discovering = discovering < PLAB_BTLE_MAXDEVICE ? discovering : -1;
					if (discovering >= 0)
					{
						_devices[discovering].id = discovering;
						delete[] _devices[discovering].name;
						_devices[discovering].name = 0;
					}
					state = DISCOVERY_ADDRESS;
					engine.prefix = statePrefixes[statePrefixIndex[state]];
				}
			}
		}
		break;
	case DISCOVERY_ENDING:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				// This should be an 'E', but since we do not have any fail state...
				state = READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
		}
		break;
	case DISCOVERY_NAME:
		if (SoftwareSerial::available())
		{
			char c = read();
			if (engine.update(c) > 0)
			{
				// Documentation says we will receive \r\n at the end of name
				if (c == '\n')
				{
					state = DISCOVERY_RUNNING;
					engine.prefix = statePrefixes[statePrefixIndex[state]];
				}
				else if (c == '\r')
				{
					int nameChars = engine.update(0);
					if (discovering > 0) {
						_devices[discovering].name = new char[nameChars + 1];
						for (int i = 0; i <= nameChars; i++)
							_devices[discovering].name[i] = engine.getMessage()[i];
					}
				}
			}
		}
		break;
	case DISCOVERY_ADDRESS:
		// Ending on either timeout or new message
		if (overTime)
		{
			int addrCount = engine.update(0);
			if (discovering >= 0) {
				delete[] _devices[discovering].address;
				_devices[discovering].address = new char[addrCount + 1];
				for (int i = 0; i <= addrCount; ++i)
					_devices[discovering].address[i] = engine.getMessage()[i];
			}
			state = discoverNames ? DISCOVERY_NAME : DISCOVERY_RUNNING;
			engine.prefix = statePrefixes[statePrefixIndex[state]];
		}
		else if (SoftwareSerial::available())
		{
			// We are dependent on no 'O' in prefix or address. Should be OK.
			char c = read();
			if (c == 'O')
			{
				int addrCount = engine.update(0);
				if (discovering >= 0) {
					delete[] _devices[discovering].address;
					_devices[discovering].address = new char[addrCount + 1];
					for (int i = 0; i <= addrCount; ++i)
						_devices[discovering].address[i] = engine.getMessage()[i];
				}
				state = discoverNames ? DISCOVERY_NAME : DISCOVERY_RUNNING;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
			engine.update(c);
		}
		break;
	case CONNECT_RUNNING:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				// Normal connect:
				// Valid: 0-5. Invalid: E/F
				// Connect last:
				// Valid: L, Invalid: E/F/N
				connected = (r >= '0' && r <= '5') || r == 'L';
				connFail = r == 'E' || r == 'F' || r == 'N';
				discoverNames = r == '1';
				state = READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
		}
		break;
	case DISCONNECTING:
		connected = workImmediately;
		state = READY;
		break;
	case RESET_IN_PROGRESS:
		if (overTime)
		{
			while (engine.update(' ') <= 0) {}
			engine.update(0);
			state = READY;
			engine.prefix = statePrefixes[statePrefixIndex[state]];
		}
		else if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				state = READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
				engine.update(r);
			}
		}
		break;
	case FACTORY_RESET_IN_PROGRESS:
		if (overTime)
		{
			while (engine.update(' ') <= 0) {}
			engine.update(0);
			state = INIT;
			engine.prefix = statePrefixes[statePrefixIndex[state]];
		}
		else if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				state = INIT;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
				engine.update(r);
			}
		}
		break;
	}
}
bool PLabBTLEController::isReady() const { return state == READY; }

void PLabBTLEController::setName(char *name)
{
	if (!isReady() || connected) return;
	state = SETTING_NAME;
	delete[] name;
	name = 0;
	print("AT+NAME");
	print(name);
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
	write("AT+DISC?");
	engine.prefix = statePrefixes[statePrefixIndex[state]];
	for (int i = 0; i < PLAB_BTLE_MAXDEVICE; ++i)
	{
		_devices[i].id = -1;
		delete[] _devices[i].name;
		_devices[i].name = 0;
		delete[] _devices[i].address;
		_devices[i].address = 0;
	}
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

void PLabBTLEController::connectDevice(int id)
{
	if (!isReady() || connected || role != CENTRAL || workImmediately) return;
	if (id >= devices()) return;
	state = CONNECT_RUNNING;
	print("AT+CONN");
	print(id);
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}
void PLabBTLEController::connectDevice(const PLabBTLEDevice &device)
{
	if (!isReady() || connected || role != CENTRAL || workImmediately || device.id < 0) return;
	state = CONNECT_RUNNING;
	print("AT+CONN");
	print(device.id);
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}
void PLabBTLEController::connectLastDevice()
{
	if (!isReady() || connected || role != CENTRAL || workImmediately) return;
	state = CONNECT_RUNNING;
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
	write("AT");
	connected = false;
	connFail = false;
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}

void PLabBTLEController::reset()
{
	if (!isReady() || connected) return;
	state = RESET_IN_PROGRESS;
	write("AT+RESET");
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}
void PLabBTLEController::factoryReset()
{
	if (!isReady() || connected) return;
	state = FACTORY_RESET_IN_PROGRESS;
	write("AT+RENEW");
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}
