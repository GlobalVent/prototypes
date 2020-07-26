
#include <string.h>
#include <pigpio.h> // Library used for I2C on Pi
#include <math.h> //Can take this out if we just bring pow() function in-line
#include <iostream>
#include <vector>

#include "I2cGenericDev.h"
#include "I2cMSxxx.h"
#include "I2cMS5803.h"
#include "I2cMS5607.h"

using namespace std;

#define PHOTON_CFG_I2C_DEV 0x60
#define PRES_I2C_SIM_ADDR  0x61
#define PSYS_I2C_SIM_ADDR  0x62

#define PRES_I2C_ADDR      0x76
#define PSYS_I2C_ADDR      0x77

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

	/**
	 * @brief Set the Loop Back mode so we can read and write the pressure sensors.
	 *        loopback mode is a test mode that disables the jamvent simulation for
	 *        reading pressure and temperature.
	 * 
	 * @param loopBack -- 1 set loopback on, 0, set it off
	 * @return int 
	 */
	int setLoopBack(unsigned loopBack) {
		uint8_t buf[1];
		buf[0] = loopBack;
		int rc = writeBlockData(SIMINTERVAL, buf, sizeof(buf));
		return (rc);

	}


	/**
	 * @brief Get the Loop Back mode
	 * 
	 * @return 0 loopBAck off, 1 loopBack on
	 */
	int getLoopBack() {
		uint8_t buf[1];
		int rc = readBlockData(SIMINTERVAL, buf, sizeof(buf));
		if (rc > 0) {
			rc = buf[0];
		}
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
	I2cMS5803 presSensor(1, PRES_I2C_ADDR);
	I2cMS5607 psysSensor(1, PSYS_I2C_ADDR);
	if (!photonCfg.open()) {
		cout << "photonCfg.initI2c Failed" << endl;
		return (1);
	}

	if (!presSensor.open()) {
		cout << "presSensor.initI2c Failed" << endl;
		return (1);
	}

	int version = photonCfg.getVersion();
	cout << "version=" << (version >> 8) << ":" << (version & 0xFF) << endl;
	
	int siminterval = photonCfg.getSiminterval();
	cout << "siminterval=" << siminterval << endl;

	rc = photonCfg.setLoopBack(true);
	if (rc != 0) {
		cout << "photonCfg.setLoopBack failed, rc=" << rc << "," << presSensor.getErrorText(rc) << endl;
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
	
	return (0);
	
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