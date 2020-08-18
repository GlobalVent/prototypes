#include "I2cAdcWithO2.h"

/**
 * @brief Takes a reading from specified ADC channel
 *        and returns an oxygen reading
 *        
 * @param channel -- ADC channel number (0-7)
 * @return float -- oxygen percentage (0-100) on success, pigpio error code on error
 */
float I2cAdcWithO2::readO2(uint8_t channel) {
	float voltage;
	
	voltage = readVoltage(channel);
	if (voltage < 0)
		return voltage;

	// apply known O2 sensor scaling
	return (voltage - _offset) * _scale;
}
