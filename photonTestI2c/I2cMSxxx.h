#ifndef __I2C_MSXXX_H__
#define __I2C_MSXXX_H__

/**
 * @brief common functionality for the MS5837 and MS607
 *        pressure devices.
 * 
 * required initialize operations.
 *     open
 *     reset
 *     readProm  -- read the coefficients..
 * 
 * close -- when done with object, destructor automatically closes  it.
 * 
 */
class I2cMSxxx : public I2cGenericDev
{
protected:
	enum temperature_units
	{
		CELSIUS,
		FAHRENHEIT,
	};

	// Define measurement type.
	enum measurement
	{
		PRESSURE = 0x00,
		TEMPERATURE = 0x10
	};

	enum {
		CMD_RESET = 0x1E,
		CMD_ADC_READ = 0x00,
		CMD_ADC_CONV_TEMP = 0x40,		// temperature conv  (combined with ADC values above)
		CMD_ADC_CONV_D1   = 0x40,		// alias		
		CMD_ADC_CONV_PRES = 0x50,		// pressure conv
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
	unsigned _pressure;				// pressure (conpensated)
	unsigned _temperature;			// temperature (compensated)
	int64_t _OFF2;					// offsets calculated with temperature
	int64_t _SENS2;					// needed for pressure.
	int64_t _OFF;
	int64_t _SENS; 

public:
	// Define constants for Conversion precision
	enum precision
	{
		ADC_256  = 0x00,
		ADC_512  = 0x02,
		ADC_1024 = 0x04,
		ADC_2048 = 0x06,
		ADC_4096 = 0x08,
		ADC_8192 = 0x0A,
	};
	I2cMSxxx() = delete;
	I2cMSxxx(unsigned bus, unsigned address)
		: I2cGenericDev(bus, address),
		_lastCv(0),
		_pressure(0),
		_temperature(0),
		_OFF2(0),
		_SENS2(0),
		_OFF(0),
		_SENS(0)
    {
		memset(_prom, 0, sizeof(_prom));
	}

	/**
	 * @brief perform the mandatory device reset before starting
	 * 		  there is a 5ms delay required before any other thing is touched.
	 * @param delay -- true, do the delay, false don't.
	 */
	int reset(bool delay=true)
	// Reset device I2C
	{
		int rc = writeByte(CMD_RESET);
		if (delay)
			gpioDelay(5000); //5 ms wait - extended this for r pi to 5ms
		return(rc);
	}

	/**
	 * @brief read the prom coeffecients.
	 * 
	 * @return int -- 0 if successful, pigpio error code if not...
	 */
	int readProm(void)
	{
		int tmp;
		uint8_t buf[2];
		int rc;
		for(unsigned i = 0; i < 8; i++){
			rc = readBlockData(CMD_PROM + (i*2), buf, sizeof(buf));
			_prom[i] = ((uint16_t)buf[0] << 8)| buf[1];
			if (rc < 0)
				return(rc);
		}
		return (rc < 0 ? rc : 0);
	}
	/**
	 * @brief retrieve the prom data (for debug display)
	 * 
	 * @param data 
	 */
	void getPromData(std::vector<uint16_t> &data) {
		data.clear();
		for(unsigned i = 0; i < 8; i++)
			data.push_back(_prom[i]);
	}

	/**
	 * @brief startTemperatureCv -- start the temperature conversion.
	 * 
	 * @param precision -- precision required (one of ADC_xxx above)
	 * @return int -- returns the number of microseconds (us) required to 
	 *                perform the conversion before the data can be read.
	 *                <= command was not able to be sent.
	 */
	virtual int startTemperatureCv(unsigned precision) {
		int rc;
		int delay = getPrecisionDelay(precision);
		if (delay < 0)
			return(delay);
		rc = writeByte(CMD_ADC_CONV_TEMP+precision);
		if (rc == 0)
			_lastCv=CMD_ADC_CONV_TEMP;
		return(rc);
	}

	/**
	 * @brief start the pressure Conversion
	 * 
	 * @param precision -- precision required (one of ADC_xxx above)
	 * @return int -- returns the number of microseconds (us) required to 
	 *                perform the conversion before the data can be read.
	 *                <= command was not able to be sent.
	 */
	virtual int startPressureCv(unsigned precision) {
		int rc;
		int delay = getPrecisionDelay(precision);;
		if (delay < 0)
			return(delay);
		rc = writeByte(CMD_ADC_CONV_PRES+precision
			);
		if (rc == 0)
			_lastCv=CMD_ADC_CONV_PRES;		// remember we did this.
		return(rc);
	}

	/**
	 * @brief read the ADC value and convert and store it in this object.
	 * 
	 * @return int 
	 */
	virtual int readAdc() {
		uint32_t adc;
		uint8_t buf[3];
		int rc = readBlockData(CMD_ADC_READ, buf, sizeof(buf));
		if (rc < 0) 
			return (rc);
		adc = ((int )buf[0]<<16) + ((int )buf[1]<<8) + buf[2];

		switch (_lastCv) {
			case CMD_ADC_CONV_TEMP:
				_temperature = convertTemperature(adc);
				break;
			case CMD_ADC_CONV_PRES:
				_pressure = convertPressure(adc);
				break;
		}
		return (0);
	}

	/**
	 * @brief convert the temperature value.
	 * 
	 * @param rawValue 
	 * @return uint32_t -- returns the temperature value.
	 */
	virtual uint32_t  convertTemperature(uint32_t rawValue) = 0;

	/**
	 * @brief convert the pressure value.
	 * 
	 * @param temperature -- current temperature to use to convert the presssure.
	 * @param rawValue -- raw pressure value to convert.
	 * @return uint32_t -- converted pressure value 
	 */
	virtual uint32_t convertPressure(uint32_t rawValue) = 0;


	/**
	 * @brief Get the Last Pressure that and conversion was done for.
	 * 
	 * @return int 
	 */ 
	virtual int getLastPressure() {
		return(_pressure);
	}

	/**
	 * @brief Get the Last Temperature value read for this sensor.
	 * 
	 * @return int 
	 */
	virtual int getLastTemperature() {
		return(_temperature);
	}

	// TEST functions for photon

	/**
	 * @brief Set the Test Pressure for loop back test
	 * 
	 * @param pressure -- pressure to set
	 * @return int  0 if OK, otherwise PI_BAD_HANDLE, PI_BAD_PARAM, or PI_I2C_WRITE_FAILED.
	 */
	virtual int setTestPressure(unsigned pressure) {
		uint8_t buf[3];
		buf[0] = (pressure>>16);
		buf[1] = (pressure>>8);
		buf[2] = pressure;
		int rc = writeBlockData(CMD_ADC_TEST_PRES,buf, sizeof(buf));
		return (rc);
	}

	/**
	 * @brief Get the Test Pressure setting
	 * 
	 * @return int 
	 */
	virtual int getTestPressure() {
		uint8_t buf[3];
		int rc = readBlockData(CMD_ADC_TEST_PRES, buf, sizeof(buf));
		if (rc > 0) {
			rc = ((int )buf[0]<<16) + ((int )buf[1]<<8) + buf[2];
		}
		return (rc);
	}

	/**
	 * @brief Set the Test Temperature 
	 * 
	 * @param temperature 
	 * @return int 
	 */
	virtual int setTestTemperature(unsigned temperature) {
		uint8_t buf[3];
		buf[0] = (_pressure>>16);
		buf[1] = (_pressure>>8);
		buf[2] = _pressure;
		int rc = writeBlockData(CMD_ADC_TEST_TEMP,buf, sizeof(buf));
		return (rc);
	}
	/**
	 * @brief Get the Test Temperature value.
	 * 
	 * @return int 
	 */
	virtual int getTestTemperature() {
		uint8_t buf[3];
		int rc = readBlockData(CMD_ADC_TEST_TEMP, buf, sizeof(buf));
		if (rc > 0) {
			rc = ((int )buf[0]<<16) + ((int )buf[1]<<8) + buf[2];
		}
		return (rc);
	}
	

private:
};
#endif
