/**
 * @brief Stand-alone executable to test I2C comms on Pi to sensors
 */

#include <string.h>
#include <pigpio.h> // Library used for I2C on Pi
#include <iostream>
#include <vector>

#include "I2cMS5607.h"
#include "I2cMS5803.h"

using namespace std;

/* I2C addresses of sensors */
#define PRES_I2C_DEV       0x77
#define PSYS_I2C_DEV       0x76


int testPresDevice() {
	int rc;
	
	// test MS5803 / PRES sensor
	I2cMS5803 presSensor(1, PRES_I2C_DEV);
	rc = presSensor.open();
	if (rc != 0) {
		cout << "presSensor.open failed, rc=" << rc << "," << presSensor.getErrorText(rc) << endl;
		return (1);
	}

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

int testPsysDevice() {
	int rc;	// return codes
	int delay;	// time we need to wait for pressure/temp reading
	
	// test MS5607 / PSYS sensor
	I2cMS5607 psysSensor(1, PSYS_I2C_DEV);
	rc = psysSensor.open();
	if (rc != 0) {
		cout << "psysSensor.open failed, rc=" << rc << "," << psysSensor.getErrorText(rc) << endl;
		return (1);
	}

	rc = psysSensor.reset();
	if (rc != 0) {
		cout << "psysSensor.reset() failed, rc=" << rc << "," << psysSensor.getErrorText(rc) << endl;
		return (1);
	}

	rc = psysSensor.readProm();
	if (rc != 0) {
		cout << "psysSensor.readProm() failed, rc=" << rc << "," << psysSensor.getErrorText(rc) << endl;
		return (1);
	}

	std::vector<uint16_t> promData;
	psysSensor.getPromData(promData);
	cout << "promData = " << endl;
	for (auto d : promData) 
		cout << "  " << d << endl;

	delay = psysSensor.startTemperatureCv(I2cMSxxx::ADC_4096);
	if (delay < 0) {
		cout << "psysSensor.startTemperatureCv() failed, rc=" << delay << "," << psysSensor.getErrorText(rc) << endl;
		return (1);
	}
	cout << "psysSensor.startTemperatureCv() = " << delay << endl;
	
	rc = psysSensor.gpioDelay(delay * 1.5);	// 50% margin just to be sure
	cout << "waited " << rc << endl;
	
	rc = psysSensor.readAdc();
	if (rc != 0) {
		cout << "psysSensor.readAdc() failed, rc=" << rc << "," << psysSensor.getErrorText(rc) << endl;
		return (1);
	}
	
	cout << "temperature is " << psysSensor.getLastTemperature() << endl;
	
	delay = psysSensor.startPressureCv(I2cMSxxx::ADC_4096);
	if (delay < 0) {
		cout << "psysSensor.startPressureCv() failed, rc=" << delay << "," << psysSensor.getErrorText(rc) << endl;
		return (1);
	}
	cout << "psysSensor.startPressureCv() = " << delay << endl;
	
	rc = psysSensor.gpioDelay(delay * 1.5);	// 50% margin just to be sure
	cout << "waited " << rc << endl;
	
	rc = psysSensor.readAdc();
	if (rc != 0) {
		cout << "psysSensor.readAdc() failed, rc=" << rc << "," << psysSensor.getErrorText(rc) << endl;
		return (1);
	}
	
	cout << "pressure is " << psysSensor.getLastPressure() << endl;
	
	return (0);
}

int main(int argc, char *argv[])
{
	int rc;
	
	cout << "initializing gpio" << endl;
	rc = gpioInitialise();
	if (rc == PI_INIT_FAILED) {
		cout << "gpioInitialise Failed" << endl;
		return (1);
	}
	cout << "pigpio version " << rc << endl;

//	testPresDevice();
	testPsysDevice();
	
	gpioTerminate();
	
	return 0;
}