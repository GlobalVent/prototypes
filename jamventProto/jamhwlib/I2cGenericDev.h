#ifndef __I2C_GENERIC_DEV_H__
#define __I2C_GENERIC_DEV_H__

#include <string>
#include <pigpio.h>

// generic i2c class
class I2cGenericDev
{
public:
	I2cGenericDev() = delete;
	I2cGenericDev(unsigned bus, unsigned address) :
		_bus(bus),
		_address(address),
		_handle(-1) {}
	virtual ~I2cGenericDev() {
		i2cClose(_handle);
	}		

	std::string getErrorText(int err);
	int open();
	void close();
	int writeByte(uint8_t data);
	int readByte();
	int readDevice(uint8_t *buf, unsigned count);
	int writeDevice(uint8_t *buf, unsigned count);
	int writeByteData(unsigned i2cReg, unsigned bVal);
	int writeWordData(unsigned i2cReg, unsigned wVal);
	int readByteData(unsigned i2cReg);
	int readWordData(unsigned i2cReg);
	int writeBlockData(unsigned i2cReg, uint8_t *buf, unsigned count);
	int readBlockData(unsigned i2cReg, uint8_t *buf, unsigned count);
	uint32_t gpioDelay(uint32_t micros);
		
protected :
	unsigned _bus;
	unsigned _address;
	int      _handle;
private:

};

#endif
