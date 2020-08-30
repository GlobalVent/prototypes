#ifndef __I2C_MSXXX_H__
#define __I2C_MSXXX_H__

#include "I2cGenericDev.h"
#include <string.h>
#include <vector>

/**
 * @brief common functionality for the MS5803 and MS5607
 *        pressure devices.
 * 
 * required initialize operations:
 *     open
 *     reset
 *     readProm  -- read the coefficients..
 * 
 * close -- when done with object, destructor automatically closes  it.
 * 
 */
class I2cMSxxx : public I2cGenericDev {
protected:
	// I2C commands
	enum {
		CMD_RESET = 0x1E,
		CMD_ADC_READ = 0x00,
		CMD_ADC_CONV_PRES = 0x40,		// pressure conv (combined with 'precision' values below)
		CMD_ADC_CONV_D1   = 0x40,		// alias		
		CMD_ADC_CONV_TEMP = 0x50,		// temperature conv (combined with 'precision' values below)
		CMD_ADC_CONV_D2   = 0x50,	    // alias

		CMD_PROM          = 0xA0,

		// extended commands for testing with photon simulation
		CMD_ADC_TEST_TEMP  = 0x10,		// set and get temperature for testing.
		CMD_ADC_TEST_PRES  = 0x11
	};

	/**
	 * @brief Get the Precision value
	 * 
	 * @param precision -- precision to look up the delay for
	 * @return int -- delay in us...
	 */
	virtual int getPrecisionDelay(unsigned precision) = 0;

	uint16_t _prom[8];
	unsigned _lastCv;		// last conversion done.
	float _pressure;				// pressure (compensated)
	float _temperature;			// temperature (compensated)
	int64_t _OFF;                   // offsets calculated with temperature
	int64_t _SENS; 					// needed for pressure.
	

public:
	// error return codes
	enum {
		MSxxx_NO_PREV_CMD = -1000,		// no start*Cv() command was called before trying to read the value with readAdc()
		MSxxx_BAD_PROM_CRC = -1001,		// CRC stored in PROM is incorrect (doesn't match PROM data)
	};
	
	// conversion precision register values
	enum precision
	{
		ADC_256  = 0x00,
		ADC_512  = 0x02,
		ADC_1024 = 0x04,
		ADC_2048 = 0x06,
		ADC_4096 = 0x08,
	};
	I2cMSxxx() = delete;
	I2cMSxxx(unsigned bus, unsigned address)
		: I2cGenericDev(bus, address),
		_lastCv(0),
		_pressure(0),
		_temperature(0),
		//_OFF2(0),
		//_SENS2(0),
		_OFF(0),
		_SENS(0)
    {
		memset(_prom, 0, sizeof(_prom));
	}

	std::string getErrorText(int err);
	int reset(bool delay = true);
	int readProm(void);
	void getPromData(std::vector<uint16_t> &data);
	virtual int startTemperatureCv(unsigned precision);
	virtual int startPressureCv(unsigned precision);
	virtual int readAdc();

	/**
	 * @brief convert the temperature value.
	 * 
	 * @param rawValue 
	 * @return float -- returns the temperature value, in °C
	 */
	virtual float convertTemperature(uint32_t rawValue) = 0;

	/**
	 * @brief convert the pressure value, using the last temperature value read
	 * 
	 * @param rawValue -- raw pressure value to convert.
	 * @return float -- converted pressure value, in bar
	 */
	virtual float convertPressure(uint32_t rawValue) = 0;


	/**
	 * @brief Get the last pressure that a conversion was done for.
	 * 
	 * @return float -- last pressure, in bar
	 */ 
	virtual float getLastPressure() {
		return(_pressure);
	}

	/**
	 * @brief Get the last temperature value read for this sensor.
	 * 
	 * @return float -- temperature, in °C
	 */
	virtual float getLastTemperature() {
		return(_temperature);
	}

private:
};
#endif
