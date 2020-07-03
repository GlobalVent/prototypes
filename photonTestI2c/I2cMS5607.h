#ifndef __I2C_MS5607_H__
#define __I2C_MS5607_H__

class I2cMS5803 : public I2cMSxxx
{
public:
	I2cMS5607() = delete;
	I2cMS5607(unsigned bus, unsigned address)
		: I2cMSxxx(bus, address) {
		memset(_prom, 0, sizeof(_prom));
	}

	/**
	 * @brief convert the temperature value.
	 * 
	 * @param rawValue 
	 * @return uint32_t -- returns the temperature value in degrees C
	 */
	uint32_t  convertTemperature(uint32_t rawValue) {
		uint32_t C1 = _prom[1];
		uint32_t C2 = _prom[2];
		uint32_t C3 = _prom[3];
		uint32_t C4 = _prom[4];
		uint32_t C5 = _prom[5];
		uint32_t C6 = _prom[6];
		uint32_t C7 = _prom[7];
        //Create Variables for calculations
        int32_t temp_calc;
        int32_t pressure_calc;

        int32_t dT;

        //Now that we have a raw temperature, let's compute our actual.
        dT = rawValue - ((int32_t)C5 << 8);
        temp_calc = (((int64_t)dT * C6) >> 23) + 2000;

        // TODO TESTING  _temperature_actual = temp_calc;

        //Now we have our first order Temperature, let's calculate the second order.
        int64_t T2, OFF2, SENS2;

        if (temp_calc < 2000)
        // If temp_calc is below 20.0C
        {
            T2 = ((int64_t)dT * dT) >> 31;
            OFF2 = 61 * ((temp_calc - 2000) * (temp_calc - 2000)) / 16;
            SENS2 = 2 * ((temp_calc - 2000) * (temp_calc - 2000));

            if(temp_calc < -1500)
            // If temp_calc is below -15.0C
            {
                OFF2  = OFF2 + 15 * ((temp_calc + 1500) * (temp_calc + 1500));
                SENS2 = SENS2 + 8 * ((temp_calc + 1500) * (temp_calc + 1500));
            }
        }
        else
        // If temp_calc is above 20.0C
        {
            T2 = 0;
            OFF2 = 0;
            SENS2 = 0;
        }

        // Now bring it all together to apply offsets

        temp_calc = temp_calc - T2;

        // offsets needed for pressure calculation...
        _OFF  = ((int64_t)C2 << 17) + (((C4 * (int64_t)dT)) >> 6);
        _SENS = ((int64_t)C1 << 16) + (((C3 * (int64_t)dT)) >> 7);
        _OFF  = _OFF  - OFF2;
        _SENS = _SENS - SENS2;

        _temperature = temp_calc;
		return (_temperature);
	}

	/**
	 * @brief convert the pressure value.
	 * 
	 * @param rawValue -- raw pressure value to convert.
	 * @return uint32_t -- converted pressure value 
	 */
	uint32_t convertPressure(uint32_t rawValue) {
        // use the calculated _SENS and _OFF values calculated with the temperature calc...
		_pressure = (((_SENS * rawValue) / 2097152 ) - _OFF) / 32768;
		return (_pressure);
	}


protected:
	/**
	 * @brief Get the Precision value
	 * 
	 * @param precision -- precision to look up the delay for
	 * @return unsigned -- delay in us...
	 */
	virtual  int getPrecisionDelay(unsigned precision) {
		int delay;
		switch (precision) 		{
			case ADC_256 : delay =   600; break;
			case ADC_512 : delay =  1170; break;
			case ADC_1024: delay =  2280; break;
			case ADC_2048: delay =  4540; break;
			case ADC_4096: delay =  9040; break;
			default: delay = -1;		// unknown precision...
		}
		return (delay);
	}
private:


};



#endif
