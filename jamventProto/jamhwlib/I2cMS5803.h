#ifndef __I2CMS5803_H__
#define __I2CMS5803_H__

#include "I2cMSxxx.h"

class I2cMS5803 : public I2cMSxxx {
public:
	I2cMS5803() = delete;
	I2cMS5803(unsigned bus, unsigned address)
		: I2cMSxxx(bus, address) {
		memset(_prom, 0, sizeof(_prom));
	}

	uint32_t  convertTemperature(uint32_t rawValue);
	uint32_t convertPressure(uint32_t rawValue);

protected:
	virtual  int getPrecisionDelay(unsigned precision);

private:
	
};

#endif
