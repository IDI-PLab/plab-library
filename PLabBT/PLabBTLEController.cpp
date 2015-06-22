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
	for (int i = 0; i < PLAB_BTLE_BSIZE; ++i) buffer[i] = 0;
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
	if (state == State::READY) return;
	unsigned long now = millis();
	bool overTime = (now - lastTime) >= PLAB_BTLE_AT_PAUSE;
	lastTime = now;

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
		else			if (SoftwareSerial::available())
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
	case State::READY:
		break;
	case State::SETTING_ROLE:
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
	state = State::DISCOVERY_RUNNING;
	write("AT+DISC?");
	engine.prefix = statePrefixes[statePrefixIndex[state]];
	for (int i = 0; i < PLAB_BTLE_MAXDEVICE; ++i) _devices[i].id = -1;
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
