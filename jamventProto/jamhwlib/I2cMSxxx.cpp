#include "I2cMSxxx.h"

/**
* @brief Get the Error get the error text associated with an error code.
* 
* @param err -- error code to get error text for
* @return std::string -- error rext...
*/
std::string I2cMSxxx::getErrorText(int err) {
	std::string errStr;
	switch (err) {
		case MSxxx_NO_PREV_CMD:  errStr = "readAdc() called w/o prev calling startTemperatureCv() or startPressureCv()"; break;
		case MSxxx_BAD_PROM_CRC: errStr = "bad CRC in PROM data"; break;
		default:  errStr = I2cGenericDev::getErrorText(err); break;
	}
	return (errStr);
}
	
/**
* @brief perform the mandatory device reset before starting
* 		  there is a 5ms delay required before any other thing is touched.
* @param delay -- true, do the delay, false don't.
* @return int -- 0 if successful, pigpio error code if not
*/
int I2cMSxxx::reset(bool delay) {
	// Reset device I2C
	int rc = writeByte(CMD_RESET);
	if (delay)
		gpioDelay(5000);  //5 ms wait - extended this for r pi to 5ms
	return(rc);
}

/**
* @brief read the prom coeffecients.
* 
* @return int -- 0 if successful, pigpio error code if not
*/
int I2cMSxxx::readProm(void) {
	int tmp;
	uint8_t buf[2];
	int rc;
	for (unsigned i = 0; i < 8; i++) {
		rc = readBlockData(CMD_PROM + (i * 2), buf, sizeof(buf));
		_prom[i] = ((uint16_t)buf[0] << 8) | buf[1];
		if (rc < 0)
			return (rc);
	}
		
	/* verify CRC code in PROM:
	 based on sample code from Measurement Specialties AN520 */
	uint16_t calcCRC = 0;	// CRC running total, calculated from PROM data
	uint16_t readCRC = _prom[7] & 0x000F;	// CRC read from PROM

	// zero out PROM's CRC for calculation
	_prom[7] &= 0xFF00;
	for (int byte_count = 0 ; byte_count < 16 ; byte_count++) {
		/* operation is performed on bytes:
		 choose LSB or MSB */
		if (byte_count % 2 == 1)
			calcCRC ^= (uint16_t)(_prom[byte_count >> 1] & 0x00FF);
		else
			calcCRC ^= (uint16_t)(_prom[byte_count >> 1] >> 8);

		for (int bit_count = 8; bit_count > 0; bit_count--) {
			if (calcCRC & (0x8000))
				calcCRC = (calcCRC << 1) ^ 0x3000;
			else
				calcCRC = (calcCRC << 1);
		}
	}
	calcCRC = (calcCRC >> 12) & 0x000F;	// extract final 4-bit remainder
		
	return (calcCRC == readCRC ? 0 : MSxxx_BAD_PROM_CRC);
}

/**
* @brief retrieve the prom data (for debug display)
* 
* @param data 
*/
void I2cMSxxx::getPromData(std::vector<uint16_t> &data) {
	data.clear();
	for (unsigned i = 0; i < 8; i++)
		data.push_back(_prom[i]);
}

/**
* @brief start the temperature conversion
* 
* @param precision -- precision required (one of ADC_xxx above)
* @return int -- returns the number of microseconds (us) required to 
*                perform the conversion before the data can be read.
*                <=0 command was not able to be sent.
*/
int I2cMSxxx::startTemperatureCv(unsigned precision) {
	int rc;
	int delay = getPrecisionDelay(precision);
	if (delay < 0)
		return (delay);
	rc = writeByte(CMD_ADC_CONV_TEMP + precision);
	if (rc != 0)
		return (rc);
	_lastCv = CMD_ADC_CONV_TEMP;
	return (delay);
}

/**
* @brief start the pressure Conversion
* NOTE: pressure conversion uses corrections calculated during
* readAdc() after a temperature conversion; must do a temperature
* conversion before doing a pressure conversion, to get correct answer
* 
* @param precision -- precision required (one of ADC_xxx above)
* @return int -- returns the number of microseconds (us) required to 
*                perform the conversion before the data can be read.
*                <=0 command was not able to be sent.
*/
int I2cMSxxx::startPressureCv(unsigned precision) {
	int rc;
	int delay = getPrecisionDelay(precision);
	if (delay < 0)
		return (delay);
	rc = writeByte(CMD_ADC_CONV_PRES + precision);
	if (rc != 0)
		return (rc);
	_lastCv = CMD_ADC_CONV_PRES; 		// remember we did this.
	return(delay);
}

/**
* @brief read the ADC value and convert and store it in this object.
* 
* @return int -- 0 if successful, pigpio error code if not
*/
int I2cMSxxx::readAdc() {
	uint32_t adc;
	uint8_t buf[3];

	int rc = readBlockData(CMD_ADC_READ, buf, sizeof(buf));
	if (rc < 0) 
		return (rc);
	if (rc != sizeof(buf))	// didn't read back the expected number of bytes
		return (PI_I2C_READ_FAILED);
	adc = ((int)buf[0] << 16) + ((int)buf[1] << 8) + buf[2];

	switch (_lastCv) {
		case CMD_ADC_CONV_TEMP:
			_temperature = convertTemperature(adc);
			break;
		case CMD_ADC_CONV_PRES:
			_pressure = convertPressure(adc);
			break;
		default:
			return (MSxxx_NO_PREV_CMD);
	}
	return (0);
}

