#include "JamHw.h"
#include <pigpio.h>
#include "I2cMS5607.h"
#include "I2cMS5803.h"
#include "I2cAdcWithO2.h"
#include "Valves.h"
#include "Speaker.h"


/**
 * @brief initializes all hardware
 * Sets up pigpio library, sensors, and FPGA/valve/speaker interfaces
 * 
 * @return int -- time (in microseconds) until O2 readings are valid on success, pigpio error code on error
 */
int JamHw::init(void) {
	int rc, wait;
	
	// pigpio library setup
	rc = gpioInitialise();
	if (rc < 0) return rc;
	
	// set up PRES pressure sensor
	_presSensor = new I2cMS5607(1, I2C_ADDR_PRES);
	rc = _presSensor->open();
	if (rc != 0) return rc;
	rc = _presSensor->reset();
	if (rc != 0) return rc;
	rc = _presSensor->readProm();
	if (rc != 0) return rc;
	
	// set up PSYS pressure sensor
	_psysSensor = new I2cMS5803(1, I2C_ADDR_PSYS);
	rc = _psysSensor->open();
	if (rc != 0) return rc;
	rc = _psysSensor->reset();
	if (rc != 0) return rc;
	rc = _psysSensor->readProm();
	if (rc != 0) return rc;
	
	// set up ADC, and conversions for O2 sensor connected to it
	_adcWithO2 = new I2cAdcWithO2(1, I2C_ADDR_ADC, ADC_VREF, O2_ZERO, O2_FS, O2_GAIN);
	rc = _adcWithO2->open();
	if (rc != 0) return rc;
	bool enableChannels[8] = { false, false, true, true, true, true, true, true }; 	// ADC channels 2-7 enabled, use internal reference
	rc = _adcWithO2->init(false, enableChannels, false);
	if (rc != 0) return rc;
	wait = rc;
	
	// set up valves
	rc = Valves::init();
	if (rc != 0) return rc;
	
	// set up alarm speaker
	rc = Speaker::init();
	if (rc != 0) return rc;
	
	return wait;
}
		
		
/**
 * @brief set the debug level to use for debugging 
 *          must have done the setLogStream below
 * 
 * @param level  -- debug level to set, 0, no debug data. 1,2,3... increasing verbosity
 */
void JamHw::setDebugLevel(unsigned level) {
}

/**
 * @brief set a logStream device to help with debugging...
 * 
 * @param ostr ostream object to send log data to.
 */
void JamHw::setLogStream(std::ostream *ostr) {
}

/**
 * @brief initiate a read of the PRES and PSYS internal temperature sensors
 * This needs to be done periodically, but not as often as the pressure
 * readings as the temperature changes more slowly
 * 
 * @return int -- returns the number of microseconds (us) required to 
 *                perform the conversion before the data can be read.
 *                <=0 command was not able to be sent.
 */
int JamHw::startTempCv() {
	int presDelay, psysDelay;
	
	// start simultaneous temperature conversions on both pressure sensors
	psysDelay = _psysSensor->startTemperatureCv(I2cMSxxx::ADC_4096);
	if (psysDelay < 0) return psysDelay;
	presDelay = _presSensor->startTemperatureCv(I2cMSxxx::ADC_4096);
	if (presDelay < 0) return presDelay;
	
	// wait time is the longest of either one
	return std::max(presDelay, psysDelay);
}

/**
 * @brief stores the PRES and PSYS temperatures
 * Call this after the temperature conversion is done; temperatures
 * are needed for compensation of the pressure readings later
 * 
 * @return int -- 0 on success, or pigpio error code
 */
int JamHw::storeTemps() {
	int rc;
	
	// perform the ADC reads on both sensors (sensor classes store the results internally)
	rc = _psysSensor->readAdc();
	if (rc < 0) return rc;
	rc = _presSensor->readAdc();
	if (rc < 0) return rc;
}

/**
 * @brief initiate a read of the PRES and PSYS pressure sensors
 * 
 * @return int -- returns the number of microseconds (us) required to 
 *                perform the conversion before the data can be read.
 *                <=0 command was not able to be sent.
 */
int JamHw::startSensorCv() {
	int presDelay, psysDelay;
	
	// start simultaneous pressure conversions on both pressure sensors
	psysDelay = _psysSensor->startPressureCv(I2cMSxxx::ADC_4096);
	if (psysDelay < 0) return psysDelay;
	presDelay = _presSensor->startPressureCv(I2cMSxxx::ADC_4096);
	if (presDelay < 0) return presDelay;
	
	// nothing needs to be done with O2 sensor, the ADC is continuously converting
	
	// wait time is the longest of either one
	return std::max(presDelay, psysDelay);
}

/**
 * @brief read the pSys sensor.
 *    this should get the pRes data from the last call to startSensorCv,
 *    with temperature compensation from the last call to startTempCv / storeTemps
 * 
 * @return float -- pSys sensor reading in absolute bars
 *                     
 */
float JamHw::getPsys() {
	int rc;
	
	rc = _psysSensor->readAdc();
	if (rc < 0) return rc;
	return _psysSensor->getLastPressure();
}

/**
 * @brief read the pRes sensor.
 *    this should get the pRes data from the last call to startSensorCv,
 *    with temperature compensation from the last call to startTempCv / storeTemps
 * 
 * @return float -- pRes sensor reading in absolute bars
 *                     
 */
float JamHw::getPres() {
	int rc;
	
	rc = _presSensor->readAdc();
	if (rc < 0) return rc;
	return _presSensor->getLastPressure();
}

/**
 * @brief getPO2 -- get the pO2 reading.
 * 
 * @return float -- O2 percentage (0-1), or error code
 */
float JamHw::getPO2() {
	return _adcWithO2->readO2(O2_CHANNEL);
}

/**
 * @brief setI2cAddress -- set alternate i2c addresses for the i2c devices.
 *          this is used for testing with the photon supplying the i2c data for
 *          the data, this should allow a complete simulation of the system 
 *          with the photon supplying a model.
 * 
 * @param pSysI2c 
 * @param pResI2c 
 * @param pO2I2c 
 */
//     void JamHw::setI2cAddress(unsigned pSysI2c,
//								 unsigned pResI2c,
//								 unsigned pO2I2c);


/**
* @brief write out the cpld keep alive sequence.
*        this will write out the keep alive sequence asynchronously
*        if the data cannot be sent right now, then buffer it 
*        and send it on the next readHwData call.  Do not block
*        during this call.
*        if more than 5 calls are made and the data is still not
*        left the building then return false and ignore the data.
* 
* @param data -- data bytes to write.
* @param len -- length of the data to write.
* 
*/
bool JamHw::writeCpldKeepalive(const uint8_t *data, unsigned len) {
}