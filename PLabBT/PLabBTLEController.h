/*
 * PLabBTLEController
 * Version 0.1, June, 2015
 * This library for the Arduino Uno simplify network communication for
 * Bluetooth LE HM-10 and HM-11 based devices. This library extends
 * the SoftwareSerial library, and wrap a controller around AT commands.
 *
 * Created by Inge Edward Halsaunet, 2015
 * Released into the public domain
 */
// !--- UNCOMMENT TO RECEIVE DEBUG OUTPUT
//#define PLAB_DEBUG

#include <Arduino.h>

#ifndef PLAB_BTLE_DEVICE_H
#define PLAB_BTLE_DEVICE_H
// Pause length before communications can start
#ifndef PLAB_BTLE_START_PAUSE
#define PLAB_BTLE_START_PAUSE 3000
#endif
#ifndef PLAB_TIMEOUT
#define PLAB_TIMEOUT 1000
#endif
// Long timeout. Used during discovery to cancel scans
#ifndef PLAB_LONG_TIMEOUT
#define PLAB_LONG_TIMEOUT 10000
#endif
// After factory reset, allow for a short delay
#ifndef PLAB_FACT_RESET_PAUSE
#define PLAB_FACT_RESET_PAUSE 250
#endif
// Maximal number of devices a discovery can find
#ifndef PLAB_BTLE_MAXDEVICE
#define PLAB_BTLE_MAXDEVICE 6
#endif
// Maximal buffer size while doing AT commands
#ifndef PLAB_BTLE_BSIZE
#define PLAB_BTLE_BSIZE 25
#endif

#include <SoftwareSerial.h>

/*
 * PLabBTLEDevice represents a discovered device.
 */
class PLabBTLEDevice {
	friend class PLabBTLEController;
private:
	int id = -1;
	char *name = 0;
	char *address = 0;
public:
	~PLabBTLEDevice()
	{
		// It is not this object that neither creates nor destroys any of its own content
	}
	// Id is controller specific identifier
	int getId() const { return id; }
	// Name of device. May be null
	const char *getName() const { return name; }
	// Address of device as string. Should not be null
	const char *getAddress() const { return address; }
};


/*
 * PLabBTLEController controls a BTLE unit through AT commands. Extends SoftwareSerial.
 * After changing a setting, it might be neccesary to run a device reset to see the information for the outside world.
 */
class PLabBTLEController : public SoftwareSerial
{
public:
	// A Role is a representation of the role a BTLE units takes
	enum Role { UNDEFINED = 0, PERIPHERAL = '0', CENTRAL = '1' };
private:

	// Implementation detail. Helper to keep track of internal state of message
	class PLabBTLEStateEngine
	{
	private:
		enum State {
			START, RECV_PREFIX, RECV_MSG
		};
		PLabBTLEStateEngine::State state = START;
		char buffer[PLAB_BTLE_BSIZE] = { 0 };
		int bLoc = 0;
		int pLoc = 0;
	public:
		char *prefix = 0;
		// -1 : ERROR
		//  0 : Receiving prefix
		//  1 : Receiving message
		// On end-of-message recive (0), return length of message
		int update(char c);
		char *getMessage();
		bool prefixRead();
#ifdef PLAB_DEBUG
		void printState();
#endif
	};

	// Main states the controller may be in. Each AT command issued causes minimum 1 state change.
	enum State {
		PRE_INIT = 0, INIT, INIT_GETNAME, INIT_GETROLE, INIT_GETDISCNAME, INIT_WORKIMM,
		READY,
		SETTING_ROLE, SETTING_NAME, SETTING_NAME_DISCOVERY, SETTING_WORKIMM,
		DISCOVERY_STARTING, DISCOVERY_RUNNING, DISCOVERY_ENDING, DISCOVERY_NAME, DISCOVERY_ADDRESS,
		CONNECT_RUNNING, DISCONNECTING,
		RESET_IN_PROGRESS, FACTORY_RESET_IN_PROGRESS
	};
	// Replies from states are often prefixed by similar string.
	char *statePrefixes[11] = {
		0, "OK+NAME:", "OK+Get:", "OK+Set:", "OK+DIS", "OK+RESET",
		"OK+RENEW", "OK+CONN", "OK+DISC", "C", ":"
	};
	// Each state has a prefix index
	const int statePrefixIndex[20] = {
		0, 0, 1, 2, 2, 2,
		0,
		3, 3, 3, 3,
		8, 4, 9, 1, 10,
		7, 0,
		5, 6
	};

	PLabBTLEStateEngine engine;

	PLabBTLEController::State state = INIT;

	// When setting name, we know when reply has ended by keeping track of length of name
	int nameLength = 0;
	int nameRcvd = 0;

	// When the last AT command was issued
	unsigned long commandIssued = 0;
	// If device has started responding to last command
	bool replyStarted = false;

	// Name of this device
	char *name = 0;
	// Discovered devices
	PLabBTLEDevice *_devices;
	// If we are connected, if we discover devices names, if we start working immediately, if a connection attempt has failed
	bool connected, discoverNames, workImmediately, connFail = false;
	// During discovery, keep track of which item we discover
	int discovering = -1;
	// Which role we have
	PLabBTLEController::Role role = UNDEFINED;
	// Initialisation
	void init(long speed);
	// Implementation detail when reading/setting name
	void updateName();
public:
	// No default constructor or copy constructor
	PLabBTLEController() = delete;
	PLabBTLEController(const PLabBTLEController &other) = delete;

	// Set up controller to pins
	PLabBTLEController(int rx, int tx) :
		SoftwareSerial(rx, tx) {
		state = PRE_INIT;
	}
	~PLabBTLEController() {
		delete[] name;
		delete[] _devices;
	}

	// Element access -> discovered device
	const PLabBTLEDevice &operator[](const int &index) const;

	// Update data structures
	void update();

	// in ready state
	bool isReady() const;

	void setName(char *name);
	const char* getName() const;

	Role getRole() const;
	void setRole(Role r);
	bool isPeripheral() const;
	bool isCentral() const;

	bool isDiscoveringNames() const;
	void setDiscoveringNames(bool disc);

	bool isWorkingImmediately() const;
	void setWorkingImmediately(bool wi);

	// Start device discovery
	void discoverDevices();
	// In discover device state
	bool isDiscoveringDevices() const;
	// How many devices are discovered
	int devices() const;
	// Delete the list of discovered devices
	void clearDiscoveredDevices();

	// Connect to device by id. Require 0 <= id < devices(). May fail if discover devices is not recently run
	void connectDevice(int id);
	// Connect to a device. Will primarily use address to connect, but can fallback to id
	void connectDevice(const PLabBTLEDevice &device);
	// Connect to a device by address
	void connectDevice(const char *address);
	// Reconnect to the last device where a successful connection was made
	void connectLastDevice();

	// Connection state checks
	bool connectionFailed() const;
	bool isConnecting() const;
	bool isConnected() const;
	void disconnect();

	// Any available information to report?
	int available()
	{
		if (state == READY)
		{
			return SoftwareSerial::available();
		}
		return 0;
	}

	// Reset device
	void reset();
	void factoryReset();

	// Start communication with the device
	void begin(long speed) { init(speed); }
#ifdef PLAB_DEBUG
	// !--- DEBUGGING OUTPUT
	int read()
	{
		int c = SoftwareSerial::read();
		if (c < 0) {
			Serial.println("R: NO CHAR");
		} else {
			Serial.print("R:");
			Serial.println((char)c);
		}
		return c;
	}
	using SoftwareSerial::write;
	size_t write(uint8_t c)
	{
		Serial.print("W:");
		Serial.println((char)c);
		return SoftwareSerial::write(c);
	}
	void printDebug()
	{
		engine.printState();
		Serial.print("engine message:");
		Serial.println(engine.getMessage());
	}
#endif
};
#endif
