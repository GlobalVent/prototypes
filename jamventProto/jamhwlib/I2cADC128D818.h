#ifndef __I2C_ADC128D818_H__
#define __I2C_ADC128D818_H__

#include "I2cGenericDev.h"

/**
 * @brief interface for the Texas Instruments ADC128D818 ADC and 'system monitor'
 * 
 * required initialization operations:
 *     open
 *     init
 */
class I2cADC128D818 : public I2cGenericDev {
public:
	I2cADC128D818() = delete;
	/**
	 * @brief Constructor with required params
	 * 
	 * @param bus -- I2C bus identifier (see pigpio library)
	 * @param address -- I2C bus address of ADC128D818
	 * @param vref -- voltage reference value (either external or internal)
	 */
	I2cADC128D818(unsigned bus, unsigned address, float vref)
		: I2cGenericDev(bus, address),
		  _vref(vref) {
	}
	
	// error return codes
	enum {
		I2C_ADC_BAD_INIT = -2000,	// timed out waiting for ADC to say it was ready
	};
	
	int init(bool extVref, bool enableChannels[8], bool enableTemp);
	int readVoltageCounts(uint8_t channel);
	float readVoltage(uint8_t channel);
	int readTemp(int16_t *temperature);
	std::string getErrorText(int err);
	
protected:
	// voltage reference value
	float _vref;
	
	// ADC resolution, in bits
	enum {
		ADC_RES = 12
	};
	
	// ADC acquisition delays for different channel types, in microseconds
	enum {
		DELAY_CHANNEL = 12200,	// normal channel (voltage reading)
		DELAY_TEMP = 3600,		// temperature reading
	};
	
	// I2C registers
	enum {
		REG_CONFIG = 0x00,	// configuration
		REG_INT_STATUS = 0x01,	// interrupt status
		REG_INT_MASK = 0x03,	// interrupt mask
		REG_CONV_RATE = 0x07,	// conversion rate
		REG_CHAN_DIS = 0x08,	// channel disable
		REG_ONE_SHOT = 0x09,	// one-shot conversion
		REG_DEEP_SHDN = 0x0A,	// deep shutdown
		REG_ADV_CONFIG = 0x0B,	// advanced configuration
		REG_BUSY_STATUS = 0x0C,	// busy status
		REG_CHAN_BASE = 0x20,	// channel readings (8x)
		REG_LIMITS_BASE = 0x2A,	// channel limit values (16x)
		REG_MFG_ID = 0x3E,	// manufacturer ID
		REG_REV_ID = 0x3F,	// revision ID
	};
	
	// bits in configuration register
	enum {
		BIT_CONFIG_START = (1 << 0),		// 0 = shutdown, 1 = startup
		BIT_CONFIG_INT_ENABLE = (1 << 1),	// 1 = enable ~INT pin
		BIT_CONFIG_INT_CLEAR = (1 << 3),	// 1 = clear ~INT pin
		BIT_CONFIG_INIT = (1 << 7),			// 1 = reset register default values
	};
	
	// bits in advanced configuration register
	enum {
		BIT_ADV_CONFIG_EXT_VREF = (1 << 0),	// 0 = internal vref, 1 = external vref
		BIT_ADV_CONFIG_MODE_SEL0 = (1 << 1),	// mode select for channel allocation (see datasheet)
		BIT_ADV_CONFIG_MODE_SEL1 = (1 << 2),
	};
	
	// bits in busy status register
	enum {
		BIT_BUSY_STATUS_BUSY = (1 << 0),	// 1 = currently doing conversion
		BIT_BUSY_STATUS_NR = (1 << 1),		// 1 = not ready, still powering up
	};
};

#endif
