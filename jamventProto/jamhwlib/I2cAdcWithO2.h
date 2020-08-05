#ifndef __I2C_ADC_WITH_O2_H__
#define __I2C_ADC_WITH_O2_H__

#include "I2cADC128D818.h"

/**
 * @brief interface for an oxygen sensor connected to the ADC128D818 ADC
 * 
 * required initialization operations:
 *     open
 *     init
 */
class I2cAdcWithO2 : I2cADC128D818 {
public:
	I2cAdcWithO2() = delete;
	/**
	 * @brief Constructor with required parameters
	 * @param bus -- I2C bus number (see pigpio library)
	 * @param address -- I2C address of ADC128D818
	 * @param vref -- ADC's voltage reference (internal or external)
	 * @param zeroPointVoltage -- voltage output of O2 sensor at 0% reading
	 * @param fullScaleVoltage -- voltage output of O2 sensor at 100% reading
	 * @param ampGain -- gain of amplifier between O2 sensor and ADC
	 */
	I2cAdcWithO2(unsigned bus, unsigned address, float vref, float zeroPointVoltage, float fullScaleVoltage, unsigned ampGain)
		: I2cADC128D818(bus, address, vref) {

			// calculate internal scale factors
			_offset = zeroPointVoltage * (float )ampGain;
			_scale = 100.0 / ((float )ampGain * (fullScaleVoltage - zeroPointVoltage));
	}
	
	float readO2(uint8_t channel);

private:
	// sensor's 0% offset, in ADC-input volts
	float _offset;
	// sensor's scale factor, in (O2 %) per ADC-input volt
	float _scale;
};

#endif