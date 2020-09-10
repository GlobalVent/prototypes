#include "I2cAdcWithO2.h"

/**
 * @brief Takes a reading from specified ADC channel
 *        and returns an oxygen reading
 *        
 * @param channel -- ADC channel number (0-7)
 * @return float -- oxygen fraction (0-1 for 0%-100%) on success, pigpio error code on error
 */
float I2cAdcWithO2::readO2(uint8_t channel) {
	float voltage, result;
	
	voltage = readVoltage(channel);
	if (voltage < 0)
		return voltage;

	// apply known O2 sensor scaling
	result = (voltage - _offset) * _scale;

	// check on the range
	if (result < 0)
		return O2_TOO_LOW;
	if (result > 1)
		return O2_TOO_HIGH;
	else
		return result;
}


/**
* @brief Get description associated with an error code.
* 
* @param err -- error code to get error text for
* @return std::string -- error text
*/
std::string I2cAdcWithO2::getErrorText(int err) {
	std::string errStr;
	switch (err) {
		case O2_TOO_LOW:  errStr = "Reading out of range (below 0%)"; break;
		case O2_TOO_HIGH: errStr = "Reading out of range (above 100%)"; break;
		default:		  errStr = I2cADC128D818::getErrorText(err); break;
	}
	return (errStr);
}