#ifndef __JAMHW_H__
#define __JAMHW_H__

#include <stdint.h>
#include <ostream>
#include "I2cMS5607.h"
#include "I2cMS5803.h"
#include "I2cAdcWithO2.h"


/**
 * @brief Hardware interface class responsible for high-level sensor readings
 * 
 * Call init() at the beginning to set up the entire hardware layer, inc. sensors, valves, FPGA interface, and alarm speaker
 * Pressure sensors need both pressure and temperature conversions done, for temperature compensation:
 *   Call startTempCv(), wait for the time it returns, then call storeTemps() to save the readings
 *   Call startSensorCv(), wait for the time it returns, then call getPsys() / getPres() / getPO2() to retrieve the pressure readings
 *     These pressure readings will be temperature-compensated based on the last temperature readings taken with storeTemps()
 *     Since pressure sensor temperatures will change much more slowly than the pressures most of the time, the
 *      startTempCv() / wait / storeTemps() cycle doesn't need to happen as often as the startSensorCv()-reading cycles
 */
class JamHw {
public:	
	JamHw() {
		_presSensor = NULL;
		_psysSensor = NULL;
		_adcWithO2 = NULL;
	};
	~JamHw() {}

	static int init(void);
	static void setDebugLevel(unsigned level);
	static void setLogStream(std::ostream *ostr); // set a log os stream device.  some place to send log info.
    static int startTempCv();
	static int storeTemps();
	static int startSensorCv();
    static float getPsys();
    static float getPres();
    static float getPO2();
//	static void setI2cAddress(unsigned pSysI2c,
//								 unsigned pResI2c,
//								 unsigned pO2I2c);
	static bool writeCpldKeepalive(const uint8_t *data, unsigned len);

private:
	// sensors
	static I2cMS5607 * _presSensor;
	static I2cMS5803 * _psysSensor;
	static I2cAdcWithO2 * _adcWithO2;
	
	// I2C addresses of sensors
	enum {
		I2C_ADDR_PRES = 0x76,	// high-pressure (MS5803) sensor
		I2C_ADDR_PSYS = 0x77,	// low-pressure (MS5607) sensor
		I2C_ADDR_ADC = 0x1D,	// ADC w/O2 sensor + others
	};
	
	// ADC and O2 sensor parameters
	constexpr static float ADC_VREF = 2.56;	// reference voltage for ADC (internal reference)
	constexpr static float O2_ZERO = 3E-3;	// O2 sensor's output voltage @ 0% oxygen
	constexpr static float O2_FS = 7E-3;	// O2 sensor's output voltage @ 100% oxygen (full-scale)
	constexpr static float O2_GAIN = 101;	// gain between the O2 sensor and ADC
	constexpr static unsigned O2_CHANNEL = 2;	// ADC channel with oxygen sensor
	
};

#endif

