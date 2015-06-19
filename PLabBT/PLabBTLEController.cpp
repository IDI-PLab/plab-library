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
    devices = new PLabBTLEDevice[PLAB_BTLE_MAXDEVICE];
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
    return devices[index];
}

void PLabBTLEController::update()
{
    if (state == State::READY) return;
    unsigned long now = millis();
    bool overTime = (now - lastTime) >= PLAB_BTLE_AT_PAUSE;
    lastTime = now;

    switch(state)
    {
    case State::INIT:
        write("AT+NAME?");
	state = State::INIT_GETNAME;
        break;
    case State::INIT_GETNAME:
        if (overTime)
	{
	    delete[] name;
	    name = new char[bLoc + 1];
	    for (int i = 0; i < bLoc; i++)
	    {
	        name[i] = buffer[i];
	    }
	    name[bLoc] = 0;
	    bLoc = 0;
	    buffer[0] = 0;
	    state = State::INIT_GETROLE;
	    write("AT+ROLE?");
	} else
	{
	    readBuffer();
	    // If we have filled buffer with OK+NAME: we can discard it
	    if (bLoc == 8 && buffer[bLoc-1] == ':')
	    {
	        if (buffer[0] == 'O' && buffer[1] == 'K' && buffer[2] == '+'
		  && buffer[3] == 'N' && buffer[4] == 'A' && buffer[5] == 'M'
		  && buffer[6] == 'E')
		    bLoc = 0;
	    }

	}
        break;
    case State::INIT_GETROLE:
        readBuffer();
	if (bLoc == 8)
	{
	    char r = 0;
	    if (buffer[0] == 'O' && buffer[1] == 'K' && buffer[2] == '+' &&
	      buffer[3] == 'G' && buffer[4] == 'e' &&buffer[5] == 't' &&
	      buffer[6] == ':')
	    {
	        r = buffer[7];
	    }
	    if (r == '0') role = Role.PERIPHERAL;
	    else (r == '1') role = Role.CENTRAL;
	    else role = Role.UNDEFINED;
	    bLoc = 0;
	    buffer[0] = 0;
	    state = State::INIT_GETDISCNAME;
	}
        break;
    case State::SETTING_NAME:
        //
        break;
    }
}
bool PLabBTLEController::isReady() const {return state == State::READY;}

void PLabBTLEController::setName(char *name) const
{
    if (!isReady()) return;
    state = SETTING_NAME;
    bLoc = 0;
    delete[] name;
    name = 0;
    print("AT+NAME");
    print(name);

}
const char* PLabBTLEController::getName() const
{
    if (state >= State::READY && state != State::SETTING_NAME)
    {
        return name;
    }
    return 0;
}

Role PLabBTLEController::getRole() const {return role;}
void PLabBTLEController::setRole(Role r);
bool PLabBTLEController::isPeripheral() const
{
    return role == Role::PERIPHERAL;
}
bool PLabBTLEController::isCentral() const
{
    return role == Role::CENTRAL;
}

bool PLabBTLEController::isDiscoveringNames() const {return discoverNames;}
void PLabBTLEController::setDiscoveringNames(bool disc);

void PLabBTLEController::discoverDevices();
bool PLabBTLEController::isDiscoveringDevices() const;
int PLabBTLEController::devices() const;

void PLabBTLEController::connectDevice(int id);
void PLabBTLEController::connectDevice(const PLabBTLEDevice &device);
void PLabBTLEController::connectLastDevice();
bool PLabBTLEController::connectionFailed() const;
bool PLabBTLEController::isConnecting() const;
bool PLabBTLEController::isConnected() const;
void PLabBTLEController::disconnect();

void PLabBTLEController::reset() const;
void PLabBTLEController::factoryReset();
