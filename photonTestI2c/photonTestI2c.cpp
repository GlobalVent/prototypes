
#include <string.h>
#include <pigpio.h> // Library used for I2C on Pi
#include <math.h> //Can take this out if we just bring pow() function in-line
#include <iostream>
#include <vector>

#include "I2cGenericDev.h"
#include "I2cMSxxx.h"
#include "I2cMS5803.h"

using namespace std;

#define PHOTON_CFG_I2C_DEV 0x60
#define PRES_I2C_TEST_DEV       0x61
#define PSYS_I2C_TEST_DEV       0x62



class I2cPhotonCfgDev : public I2cGenericDev 
{
public:
	I2cPhotonCfgDev() = delete;
	I2cPhotonCfgDev(unsigned bus, unsigned address)
		: I2cGenericDev(bus, address) {}

	/**
	 * @brief Get the Version word
	 * 
	 * @return int -- version word, or error code if error
	 */
	int getVersion() {
		uint8_t buf[2];
		int rc = readBlockData(VERSION, buf, sizeof(buf));
		if (rc > 0) {
			rc = (buf[0]<<8) + buf[1];
		}
		return (rc);
	}
	/**
	 * @brief Get the Siminterval from the slave i2c device.
	 * 
	 * @return int 
	 */
	int getSiminterval() {
		uint8_t buf[2];
		int rc = readBlockData(SIMINTERVAL, buf, sizeof(buf));
		if (rc > 0) {
			rc = (buf[0]<<8) + buf[1];
		}
		return (rc);
	}
	int setSimInterval(unsigned simInterval) {
		uint8_t buf[2];
		buf[0] = (simInterval>>8);
		buf[1] = simInterval;
		int rc = writeBlockData(SIMINTERVAL, buf, sizeof(buf));
		return (rc);
	}

protected:
	enum {
		VERSION = 0, 
		        // return with the version number of this simulation (replies with 2 bytes, major and minor)
		SIMINTERVAL = 1     // set the sim interval in milliseconds.
	};

private:
};


int testI2cDevices()
{
	int rc;
	cout << "callling initI2c" << endl;
	I2cPhotonCfgDev photonCfg(1, PHOTON_CFG_I2C_DEV);
	I2cMS5803 presSensor(1, PRES_I2C_TEST_DEV);
	if (!photonCfg.open()) {
		cout << "photonCfg.initI2c Failed" << endl;
		return (1);
	}

	if (!presSensor.open()) {
		cout << "presSensor.initI2c Failed" << endl;
		return (1);
	}

	// do the initialization thing for the sensors.
	rc = presSensor.reset();
	if (rc != 0) {
		cout << "presSensor.reset() failed, rc=" << rc << "," << presSensor.getErrorText(rc) << endl;
		return (1);
	}

	rc = presSensor.readProm();
	if (rc != 0) {
		cout << "presSensor.readProm() failed, rc=" << rc << "," << presSensor.getErrorText(rc) << endl;
		return (1);
	}

	std::vector<uint16_t> promData;
	presSensor.getPromData(promData);
	cout << "promData = " << endl;
	for (auto d : promData) 
		cout << "  " << d << endl;

	rc = presSensor.getTestPressure();
	if (rc < 0) {
		cout << "presSensor.getTestPressure() failed, rc=" << rc << "," << presSensor.getErrorText(rc) << endl;
		return (1);
	}
	cout << "presSensor.getTestPressure() = " << rc << endl;

	rc = presSensor.setTestPressure(2000);
	if (rc < 0) {
		cout << "presSensor.setTestPressure() failed, rc=" << rc << "," << presSensor.getErrorText(rc) << endl;
		return (1);
	}
	
	rc = presSensor.getTestPressure();
	if (rc < 0) {
		cout << "presSensor.getTestPressure() failed, rc=" << rc << "," << presSensor.getErrorText(rc) << endl;
		return (1);
	}
	cout << "presSensor.getTestPressure() = " << rc << endl;
	

#if 0
	int version = photonCfg.getVersion();
	cout << "version=" << (version >> 8) << ":" << (version & 0xFF) << endl;
	
	int siminterval = photonCfg.getSiminterval();
	cout << "siminterval=" << siminterval << endl;
#endif
	
}
int main(int argc, char *argv[])
{
	if (gpioInitialise() < 0)	{
		cout << "gpioInitialize Failed" << endl;
		return (1);
	}
	
	testI2cDevices();
	
	gpioTerminate();
	
	return 0;
}