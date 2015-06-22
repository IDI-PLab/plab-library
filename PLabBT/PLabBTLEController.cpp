#include "PLabBTLEController.h"
/*
class PLabBTLEDevice {
friend class PLabBTLEController;
private:
int id = -1;
char *name = 0;
char *address = 0;
public:
int id() const;
const char *name() const;
const char *address() const;
}
*/

/*
class PLabBTLEController : public SoftwareSerial
{
private:
enum State {
INIT, INIT_GETNAME, INIT_GETROLE, INIT_GETDISCNAME,
READY,
SETTING_ROLE, SETTING_NAME, SETTING_NAME_DISCOVERY,
DISCOVERY_RUNNING, CONNECT_RUNNING, DISCONNECTING,
RESET_IN_PROGRESS, FACTORY_RESET_IN_PROGRESS
};

state = State::INIT;

char buffer[PLAB_BTLE_BSIZE];
int bLoc = 0;

char *name;
PLabBTLEDevice *devices;
bool connected, discoverNames;
Role role = Role::UNDEFINED;
public:

enum Role {UNDEFINED=0, PERIPHERAL='0', CENTRAL='1'};

}
*/
// #define PLAB_BTLE_BSIZE 25
void PLabBTLEController::init(long speed)
{
	begin(speed);
	state = State::INIT;
	bLoc = 0;
	name = 0;
	role = Role::UNDEFINED;
	_devices = new PLabBTLEDevice[PLAB_BTLE_MAXDEVICE];
}
bool PLabBTLEController::readBuffer()
{
	if (SoftwareSerial::available() && bLoc < (PLAB_BTLE_BSIZE - 1))
	{
		buffer[bLoc++] = read();
		buffer[bLoc] = 0;
	}
	return bLoc < PLAB_BTLE_BSIZE;
}
const PLabBTLEController::PLabBTLEDevice &operator[](const int &index) const
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
	unsigned long now = millis();
	// If the current state is not dependent on overTime, update lastTime
	if (state == State::INIT ||
		state == State::INIT_GETROLE ||
		state == State::INIT_GETDISCNAME ||
		state == State::INIT_WORKIMM ||
		state == State::SETTING_ROLE ||
		state == State::SETTING_NAME_DISCOVERY ||
		state == State::SETTING_WORKIMM ||
		state == State::DISCOVERY_STARTING ||
		state == State::DISCOVERY_RUNNING ||
		state == State::DISCOVERY_ENDING ||
		state == State::DISCOVERY_NAME ||
		state == State::CONNECT_RUNNING ||
		state == State::DISCONNECTING)
	{
		lastTime = now;
	}

	// READY should not handle anything but lastTime update
	if (state == State::READY) return;

	bool overTime = (now - lastTime) >= PLAB_BTLE_AT_PAUSE;

	switch (state)
	{
	case State::INIT:
		state = State::INIT_GETNAME;
		write("AT+NAME?");
		engine.prefix = statePrefixes[statePrefixIndex[state]];
		break;
	case State::INIT_GETNAME:
		if (overTime)
		{
			updateName();
			state = State::INIT_GETROLE;
			write("AT+ROLE?");
			engine.prefix = statePrefixes[statePrefixIndex[state]];
		}
		else if (SoftwareSerial::available())
			engine.update(read());
		break;
	case State::INIT_GETROLE:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				if (r == '0') role = Role.PERIPHERAL;
				else if (r == '1') role = Role.CENTRAL;
				else role = Role.UNDEFINED;
				state = State::INIT_GETDISCNAME;
				write("AT+SHOW?");
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
		}
		break;
	case State::INIT_GETDISCNAME:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				discoverNames = r == '1';
				state = State::INIT_WORKIMM;
				write("AT+IMME?");
				engine.prefix = statePrefixes[statePrefixIndex[state]];

			}
		}
		break;
	case State::INIT_WORKIMM:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				workImmediately = r == '0';
				state = State::READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];

			}
		}
		break;
	case State::SETTING_ROLE:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				if (r == '0') role = Role.PERIPHERAL;
				else if (r == '1') role = Role.CENTRAL;
				else role = Role.UNDEFINED;
				state = State::READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
		}
		break;
	case State::SETTING_NAME:
		if (overTime)
		{
			updateName();
			state = State::READY;
			engine.prefix = statePrefixes[statePrefixIndex[state]];
		}
		else if (SoftwareSerial::available())
			engine.update(read());
		break;
	case State::SETTING_NAME_DISCOVERY:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				discoverNames = r == '1';
				state = State::READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];

			}
		}
		break;
	case State::SETTING_WORKIMM:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				workImmediately = r == '0';
				state = State::READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
		}
		break;
	case State::DISCOVERY_STARTING:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				state = r == 'S' ? State::DISCOVERY_RUNNING : State::READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
		}
		break;
	case State::DISCOVERY_RUNNING:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				// Assuming non-hex numbering of devices
				if (r == 'C')
				{
					state = State::DISCOVERY_ENDING;
					engine.prefix = statePrefixes[statePrefixIndex[state]];
					enging.update(r);
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
					state = State::DISCOVERY_ADDRESS;
					engine.prefix = statePrefixes[statePrefixIndex[state]];
				}
			}
		}
		break;
	case State::DISCOVERY_ENDING:
		if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				// This should be an 'E', but since we do not have any fail state...
				state = State::READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
		}
		break;
	case State::DISCOVERY_NAME:
		if (SoftwareSerial::available())
		{
			char c = read();
			if (engine.update(c) > 0)
			{
				// Documentation says we will receive \r\n at the end of name
				if (c == '\n')
				{
					state = State::DISCOVERY_RUNNING;
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
	case State::DISCOVERY_ADDRESS:
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
			state = discoverNames ? State::DISCOVERY_NAME : State::DISCOVERY_RUNNING;
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
				state = discoverNames ? State::DISCOVERY_NAME : State::DISCOVERY_RUNNING;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
			engine.update(c);
		}
		break;
	case State::CONNECT_RUNNING:
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
				state = State::READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
			}
		}
		break;
	case State::DISCONNECTING:
		connected = workImmediately;
		state = State::READY;
		break;
	case State::RESET_IN_PROGRESS:
		if (overTime)
		{
			while (engine.update(' ') <= 0) {}
			engine.update(0);
			state = State::READY;
			engine.prefix = statePrefixes[statePrefixIndex[state]];
		}
		else if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				state = State::READY;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
				engine.update(r);
			}
		}
		break;
	case State::FACTORY_RESET_IN_PROGRESS:
		if (overTime)
		{
			while (engine.update(' ') <= 0) {}
			engine.update(0);
			state = State::INIT;
			engine.prefix = statePrefixes[statePrefixIndex[state]];
		}
		else if (SoftwareSerial::available())
		{
			if (engine.update(read()) > 0)
			{
				engine.update(0);
				char r = engine.getMessage()[0];
				state = State::INIT;
				engine.prefix = statePrefixes[statePrefixIndex[state]];
				engine.update(r);
			}
		}
		break;
	}
}
bool PLabBTLEController::isReady() const { return state == State::READY; }

void PLabBTLEController::setName(char *name) const
{
	if (!isReady() || connected) return;
	state = State::SETTING_NAME;
	delete[] name;
	name = 0;
	print("AT+NAME");
	print(name);
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}
const char* PLabBTLEController::getName() const
{
	if (state >= State::READY && state != State::SETTING_NAME)
	{
		return name;
	}
	return 0;
}

Role PLabBTLEController::getRole() const { return role; }
void PLabBTLEController::setRole(Role r) {
	if (!isReady() || r == Role::UNDEFINED || connected) return;
	state = State::SETTING_ROLE;
	print("AT+ROLE");
	print((char)r);
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}
bool PLabBTLEController::isPeripheral() const
{
	return role == Role::PERIPHERAL;
}
bool PLabBTLEController::isCentral() const
{
	return role == Role::CENTRAL;
}

bool PLabBTLEController::isDiscoveringNames() const { return discoverNames; }
void PLabBTLEController::setDiscoveringNames(bool disc)
{
	if (!isReady() || connected) return;
	state = State::SETTING_NAME_DISCOVERY;
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
	state = State::SETTING_WORKIMM;
	if (wi)
		write("AT+IMME0");
	else
		write("AT+IMME1");
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}

void PLabBTLEController::discoverDevices()
{
	if (!isReady() || connected || role != Role::CENTRAL || workImmediately) return;
	state = State::DISCOVERY_STARTING;
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
bool PLabBTLEController::isDiscoveringDevices() const { return state == State::DISCOVERY_RUNNING; }
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
	if (!isReady() || connected || role != Role::CENTRAL || workImmediately) return;
	if (id >= devices()) return;
	state = State::CONNECT_RUNNING;
	print("AT+CONN");
	print(id);
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}
void PLabBTLEController::connectDevice(const PLabBTLEDevice &device)
{
	if (!isReady() || connected || role != Role::CENTRAL || workImmediately || device.id < 0) return;
	state = State::CONNECT_RUNNING;
	print("AT+CONN");
	print(device.id);
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}
void PLabBTLEController::connectLastDevice()
{
	if (!isReady() || connected || role != Role::CENTRAL || workImmediately) return;
	state = State::CONNECT_RUNNING;
	write("AT+CONNL");
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}
bool PLabBTLEController::connectionFailed() const { return connFail; }
bool PLabBTLEController::isConnecting() const { return state == State::CONNECT_RUNNING; }
bool PLabBTLEController::isConnected() const { return connected; }
void PLabBTLEController::disconnect()
{
	if (!isReady() || !connected || role != Role::CENTRAL) return;
	state = State::DISCONNECTING;
	write("AT");
	connected = false;
	connFail = false;
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}

void PLabBTLEController::reset() const
{
	if (!isReady() || connected) return;
	state = State::RESET_IN_PROGRESS;
	write("AT+RESET");
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}
void PLabBTLEController::factoryReset()
{
	if (!isReady() || connected) return;
	state = State::FACTORY_RESET_IN_PROGRESS;
	write("AT+RENEW");
	engine.prefix = statePrefixes[statePrefixIndex[state]];
}
