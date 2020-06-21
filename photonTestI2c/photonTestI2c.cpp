
#include <pigpio.h> // Library used for I2C on Pi
#include <math.h> //Can take this out if we just bring pow() function in-line
#include <iostream>

using namespace std;

#define PHOTON_CFG_I2C_DEV 0x60
#define PRES_I2C_DEV       0x61

// generic i2c class...
class I2cGenericDev
{
public:
	I2cGenericDev() = delete;
	I2cGenericDev(unsigned bus, unsigned address) :
		_bus(bus),
		_address(address),
		_handle(0) {}
	virtual ~I2cGenericDev() {
		if (_handle)
			i2cClose(_handle);
	}		
	/**
	 * @brief initialize our connection to the i2c device.
	 * 
	 * @return true -- device successfully opened
	 * @return false  -- device closed.
	 */
	bool initI2c()
	{	
		_handle = i2cOpen(_bus, _address, 0); // NOTE: Assumes Pi with i2cbus 1 (all after model B)
		return(_handle != 0);
	}

	/**
	 * @brief write a single byte to the i2c device.
	 * 
	 * @param data -- data to write.
	 * @return int -- -- 0 if OK, otherwise PI_BAD_HANDLE, PI_BAD_PARAM, or PI_I2C_WRITE_FAILED.
	 */
	int writeByte(uint8_t data) {
	  int rc = i2cWriteByte(_handle,data);
	  return(rc);
	}

	/**
	 * @brief read a single i2c byte for the i2c device.
	 * 
	 * @return int -- the byte read (>=0) if OK, otherwise PI_BAD_HANDLE, or PI_I2C_READ_FAILED.
	 */
	int readByte() {
		int rc = i2cReadByte(_handle);
		return(rc);
	}

	/**
	 * @brief writeByteData
	 * 
	 * @param i2cReg -- i2c register to write data to. (basically a command followed by data)
	 * @param bVal -- value to send
	 * @return int -- 0 if OK, otherwise PI_BAD_HANDLE, PI_BAD_PARAM, or PI_I2C_WRITE_FAILED.
	 */
	int writeByteData(unsigned i2cReg, unsigned bVal) {
		int rc = i2cWriteByteData(_handle, i2cReg, bVal);
		return(rc);
	}

	/**
	 * @brief i2c write word data to an i2c reg
	 * 
	 * @param i2cReg -- i2c register to write data to. (basically a command followed by data)
	 * @param wVal -- word value to send.
	 * @return int -- 0 if OK, otherwise PI_BAD_HANDLE, PI_BAD_PARAM, or PI_I2C_WRITE_FAILED.
	 */
	int writeWordData(unsigned i2cReg, unsigned wVal) {
		int rc = i2cWriteWordData(_handle, i2cReg, wVal);
		return(rc);
	}

	/**
	 * @brief 
	 * 
	 * @param i2cReg -- reads a single byte from the specified register of the device
	 * @return int -- he byte read (>=0) if OK, otherwise PI_BAD_HANDLE, PI_BAD_PARAM, or PI_I2C_READ_FAILED.

	 */
	int readByteData(unsigned i2cReg) {
		int rc = i2cReadByteData(_handle, i2cReg);
		return(rc);
	}

	/**
	 * @brief 
	 * 
	 * @param i2cReg -- reads a single 16 bit word from the specified register of the device
	 * @return int --  the word read (>=0) if OK, otherwise PI_BAD_HANDLE, PI_BAD_PARAM, or PI_I2C_READ_FAILED.
	 */
	int readWordData(unsigned i2cReg) {
		int rc = i2cReadWordData(_handle, i2cReg);
		return(rc);
	}

	/**
	 * @brief This writes up to 32 bytes to the specified register of the device
	 * 
	 * @param i2cReg register to write to.
	 * @param buf -- buffer to write from.
	 * @return int 0 if OK, otherwise PI_BAD_HANDLE, PI_BAD_PARAM, or PI_I2C_WRITE_FAILED.
	 */
	int writeBlockData(unsigned handle, unsigned i2cReg, char *buf, unsigned count) {
		int rc = i2cWriteI2CBlockData(_handle, i2cReg, buf, count);
		return(rc);
	}

	/**
	 * @brief This reads count bytes from the raw device into buf.
	 * 
	 * @param i2cReg -- register to read from.
	 * @param buf 
	 * @return int 
	 */
	int readBlockData(unsigned i2cReg, char *buf, unsigned count) {
		int rc = i2cReadI2CBlockData(_handle, i2cReg, buf, count);
		return(rc);
	}

	/**
	 * @brief Delays for at least the number of microseconds specified by micros.
	 * 
	 * @param micros 
	 * @return uint32_t -- actual time we were asleep..
	 */
	uint32_t gpioDelay(uint32_t micros) {
		return(::gpioDelay(micros));
	}

protected :
	unsigned _bus;
	unsigned _address;
	int      _handle;
private:

};


class I2cPhotonCfgDev : public I2cGenericDev 
{
public:
	I2cPhotonCfgDev() = delete;
	I2cPhotonCfgDev(unsigned bus, unsigned address)
		: I2cGenericDev(bus, address) {}
	;
protected:
	enum {
		VERSION = 0, 
		        // return with the version number of this simulation (replies with 2 bytes, major and minor)
		SIMINTERVAL = 1     // set the sim interval in milliseconds.
	};

	/**
	 * @brief Get the Version word
	 * 
	 * @return int -- version word, or error code if error
	 */
	int getVersion() {
		int rc = readWordData(VERSION);
		return (rc);
	}
	/**
	 * @brief Get the Siminterval from the slave i2c device.
	 * 
	 * @return int 
	 */
	int getSiminterval() {
		int rc = readWordData(SIMINTERVAL);
		return (rc);
	}
	int setSimInterval(unsigned simInterval) {
		int rc = writeWordData(SIMINTERVAL, simInterval);
		return (rc);
	}

private:
};


int main(int argc, char *argv[])
{
	I2cPhotonCfgDev photonCfg(1, PHOTON_CFG_I2C_DEV);
	char sz[] = "Hello, World!";	//Hover mouse over "sz" while debugging to see its contents
	cout << sz << endl;	//<================= Put a breakpoint here
	return 0;
}