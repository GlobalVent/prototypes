#include "I2cADC128D818.h"

/**
 * @brief Initialize the ADC128D818, in Continuous Conversion Mode
 * Since we don't care about power consumption, easiest is to let it
 * do free-running conversions of all enabled channels, and read back
 * the results when needed;
 * Total conversion time is 12.2 ms per channel + 3.6 ms for temperature,
 * so with all channels enabled, readings will be out of date by ~100 ms max.
 * After ADC is initialized, need to wait long enough for it to run conversions
 * on all (enabled) channels before trying to read back the channel values with
 * readVoltage() etc. - returns this initial wait time to allow proper timing
 * 
 * @param extVref -- false = use internal 2.56V reference, true = use external voltage reference
 *         enableChannels -- true to enable continuous readings from corresponding channel
 *         enableTemp -- true to enable continuous readings from internal temperature sensor;
 *         if this is true then channel 7 voltage will not be available
 * @return int -- time to wait (microseconds) before first set of readings is valid, or pigpio error code if unsuccessful
 */
int I2cADC128D818::init(bool extVref, bool enableChannels[8], bool enableTemp) {
	int rc;
	int readingDelay = 0;
	int startupWaitCount = 0;
	
	/* startup sequence from datasheet */
	// wait for the device to signal it's ready ("not ready" bit turns off)
	do {
		gpioDelay(1000);
		rc = readByteData(REG_BUSY_STATUS);
		if (rc < 0)
			return rc;
		
		// count the number of times we've had to wait for the "not ready" bit
		// (this is always at least 1 for the first time checking)
		startupWaitCount++;
		if (startupWaitCount >= 100)
			return I2C_ADC_BAD_INIT;
	} while (rc & BIT_BUSY_STATUS_NR);
	
	// reset the registers just in case we're re-initializing, and can't depend on the power-on reset
	rc = writeByteData(REG_CONFIG, BIT_CONFIG_INIT);
	if (rc < 0)
		return rc;
	
	/* set up advanced config register with ext vref, and put into
		mode 0 (channel 7 = temp) if temp enabled, mode 1 (all channels
		enabled, single-ended) otherwise */
	rc = writeByteData(REG_ADV_CONFIG, (extVref ? BIT_ADV_CONFIG_EXT_VREF : 0) | (enableTemp ? 0 : BIT_ADV_CONFIG_MODE_SEL0));
	if (rc < 0)
		return rc;
	// choose channels to enable: build a bit mask
	uint8_t reg = 0;
	for (int i = 0; i < 7; i++) {
		reg |= (enableChannels[i] ? 0 : 1 << i);
		readingDelay += enableChannels[i] ? DELAY_CHANNEL : 0;
	}
	if (enableTemp) {	// channel 7 is special, as it's the temperature channel when temperature readings are enabled
	   reg |= (1 << 8);   	// channel 7 is always on if temperature is enabled
	   readingDelay += DELAY_TEMP;
	}
	else {
		reg |= enableChannels[7] ? 0 : 1 << 7; 	// otherwise, it's configurable like all the others
		readingDelay += enableChannels[7] ? DELAY_CHANNEL : 0;
	}
		
	rc = writeByteData(REG_CHAN_DIS, reg);
	if (rc < 0)
		return rc;
	// put into continuous conversion mode
	rc = writeByteData(REG_CONV_RATE, 0x01);
	if (rc < 0)
		return rc;
	// program REG_LIMIT... here if using
	// start conversions
	rc = writeByteData(REG_CONFIG, BIT_CONFIG_START);
	if (rc < 0)
		return rc;
	
	return readingDelay;
}

/**
 * @brief Reads last conversion result from a single channel, in ADC counts
 * 
 * @param channel -- channel number (0-7); if temperature readings were enabled
 *                    (enableTemp=true) when init() was called, then channel 7
 *                    is not available
 * @return int --result in ADC counts if successful (>= 0), pigpio error code if not
 */
int I2cADC128D818::readVoltageCounts(uint8_t channel) {
	int rc;

	uint8_t data[2];
	rc = readBlockData(REG_CHAN_BASE + channel, data, 2);
	if (rc < 0)
		return rc;

	// adjust for left-aligned 12 bits from ADC
	rc = ((int)data[0] << 4) + ((int)data[1] >> 4);

	return rc;
}

/**
 * @brief Reads last conversion result from a single channel, in volts
 * 
 * @param channel -- channel number (0-7); if temperature readings were enabled
 *                    (enableTemp=true) when init() was called, then channel 7
 *                    is not available
 * @return int --result in volts if successful (>= 0), pigpio error code if not
 */
float I2cADC128D818::readVoltage(uint8_t channel) {
	int rc;
	
	rc = readVoltageCounts(channel);
	if (rc < 0)
		return rc;
	
	// scale by known reference voltage and ADC resolution
	return ((float )rc + 0.5) * (_vref / (1 << ADC_RES));
}

/**
 * @brief Reads last conversion result from ADC128D818's internal temperature sensor;
 *        if temperature readings were not enabled (enableTemp=false) when init() was
 *        called, the result will be meaningless
 * 
 * @param temperature -- var to store temperature result, in half-°C units (50 = 25°C)
 * @return int -- 0 if successful, pigpio error code if not
 */
int I2cADC128D818::readTemp(int16_t *temperature) {
	int rc;
	
	if (temperature == NULL)
		return PI_BAD_PARAM;

	uint8_t data[2];
	rc = readBlockData(REG_CHAN_BASE + 7, data, 2);
	if (rc < 0)
		return rc;

	// data is 9 bits, MSB-first, left-aligned
	rc = ((int )data[0] << 1) + ((int )data[1] >> 7);
	
	// result is 9-bit signed, so convert negative temps for 16-bit signed result
	if (rc >= 256)
		*temperature = rc - 512;
	else
		*temperature = rc;
	
	return 0;
}


/**
* @brief Get description associated with an error code.
* 
* @param err -- error code to get error text for
* @return std::string -- error text
*/
std::string I2cADC128D818::getErrorText(int err) {
	std::string errStr;
	switch (err) {
		case I2C_ADC_BAD_INIT:  errStr = "Timed out waiting for 'not ready' bit"; break;
		default:		  errStr = I2cGenericDev::getErrorText(err); break;
	}
	return (errStr);
}
