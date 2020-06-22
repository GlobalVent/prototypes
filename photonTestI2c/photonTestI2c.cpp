
#include <string.h>
#include <pigpio.h> // Library used for I2C on Pi
#include <math.h> //Can take this out if we just bring pow() function in-line
#include <iostream>
#include <vector>

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
		return(_handle >= 0);
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
	 * @brief reads count bytes from the raw device into buf.
	 * 
	 * @param buf -- buffer to read data into
	 * @param count -- count of data to read
	 * @return int -- count (>0) if OK, otherwise PI_BAD_HANDLE, PI_BAD_PARAM, or PI_I2C_READ_FAILED.
	 */ 
	int readDevice(uint8_t *buf, unsigned count) {
		int rc = i2cReadDevice(_handle, (char*)buf, count);
		return(rc);
	}


	/**
	 * @brief  writes count bytes from buf to the raw device.
	 * 
	 * @param buf -- buffer to write data from
	 * @param count -- byte count to write.
	 * @return int -- 0 if OK, otherwise PI_BAD_HANDLE, PI_BAD_PARAM, or PI_I2C_WRITE_FAILED.
	 */ 
	int writeDevice(uint8_t *buf, unsigned count) {
		int rc = i2cWriteDevice(_handle, (char*)buf, count);
		return (rc);
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
	int writeBlockData(unsigned i2cReg, uint8_t *buf, unsigned count) {
		int rc = i2cWriteI2CBlockData(_handle, i2cReg, (char*)buf, count);
		return(rc);
	}

	/**
	 * @brief This reads count bytes from the raw device into buf.
	 * 
	 * @param i2cReg -- register to read from.
	 * @param buf 
	 * @return int 
	 */
	int readBlockData(unsigned i2cReg, uint8_t *buf, unsigned count) {
		int rc = i2cReadI2CBlockData(_handle, i2cReg, (char*)buf, count);
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

/**
 * @brief common functionality for the MS5837 and MS607
 *        pressure devices.
 * 
 * required initialize operations.
 *     initI2c
 *     reset
 *     readProm  -- read the coefficients..
 * 
 * 
 * 
 */
class I2cMSxxx : public I2cGenericDev
{
protected:
	enum temperature_units
	{
		CELSIUS,
		FAHRENHEIT,
	};

	// Define measurement type.
	enum measurement
	{
		PRESSURE = 0x00,
		TEMPERATURE = 0x10
	};

	enum {
		CMD_RESET = 0x1E,
		CMD_ADC_READ = 0x00,
		CMD_ADC_CONV_TEMP = 0x40,		// temperature conv  (combined with ADC values above)
		CMD_ADC_CONV_D1   = 0x40,		// alias		
		CMD_ADC_CONV_PRES = 0x50,		// pressure conv
		CMD_ADC_CONV_D2   = 0x50,	    // alias

		CMD_PROM          = 0xA0,

		// extended commands for testing with photon simulation
		CMD_ADC_TEST_TEMP  = 0x10,		// set and get temperature for testing.
		CMD_ADC_TEST_PRES  = 0x11
	};

	/**
	 * @brief Get the Precision value
	 * 
	 * @param precision -- precision to look up the delay for
	 * @return int -- delay in us...
	 */
	virtual int getPrecisionDelay(unsigned precision) = 0;

	uint8_t _prom[8];
	unsigned _lastCv;		// last conversion done.
	unsigned _pressure_raw;			// current pressure value (raw uncompensated reading)
	unsigned _temperature_raw;		// current temperature value (last value read)
	unsigned _pressure;				// pressure (conpensated)
	unsigned _temperature;			// temperature (compensated)

public:
	// Define constants for Conversion precision
	enum precision
	{
		ADC_256  = 0x00,
		ADC_512  = 0x02,
		ADC_1024 = 0x04,
		ADC_2048 = 0x06,
		ADC_4096 = 0x08,
		ADC_8192 = 0x0A,
	};
	I2cMSxxx() = delete;
	I2cMSxxx(unsigned bus, unsigned address)
		: I2cGenericDev(bus, address),
		_lastCv(0),
		_pressure_raw(0),
		_temperature_raw(0),
		_pressure(0),
		_temperature(0) {
		memset(_prom, 0, sizeof(_prom));
	}

	/**
	 * @brief perform the mandatory device reset before starting
	 * 		  there is a 5ms delay required before any other thing is touched.
	 * @param delay -- true, do the delay, false don't.
	 */
	void reset(bool delay=true)
	// Reset device I2C
	{
		writeByte(CMD_RESET);
		if (delay)
			gpioDelay(5000); //5 ms wait - extended this for r pi to 5ms
	}

	/**
	 * @brief read the prom coeffecients.
	 * 
	 * @return int8_t 
	 */
	int readProm(void)
	{
		int tmp;
		uint8_t buf[2];
		int rc;
		for(unsigned i = 0; i < 8; i++){
			rc = readBlockData(CMD_PROM + (i*2), buf, sizeof(buf));
			_prom[i] = (buf[0] << 8)| buf[1];
			if (rc < 0)
				return(rc);
		}
		return rc;
	}
	/**
	 * @brief retrieve the prom data (for debug display)
	 * 
	 * @param data 
	 */
	void getPromData(std::vector<uint8_t> &data) {
		data.clear();
		data.insert(data.end(), _prom, _prom + sizeof(_prom)/sizeof(*_prom));
	}

	/**
	 * @brief startTemperatureCv -- start the temperature conversion.
	 * 
	 * @param precision -- precision required (one of ADC_xxx above)
	 * @return int -- returns the number of microseconds (us) required to 
	 *                perform the conversion before the data can be read.
	 *                <= command was not able to be sent.
	 */
	int startTemperatureCv(unsigned precision) {
		int rc;
		int delay = getPrecisionDelay(precision);
		if (delay < 0)
			return(delay);
		rc = writeByte(CMD_ADC_CONV_TEMP+precision);
		if (rc == 0)
			_lastCv=CMD_ADC_CONV_TEMP;
		return(rc);
	}

	/**
	 * @brief start the pressure Conversion
	 * 
	 * @param precision -- precision required (one of ADC_xxx above)
	 * @return int -- returns the number of microseconds (us) required to 
	 *                perform the conversion before the data can be read.
	 *                <= command was not able to be sent.
	 */
	int startPressureCv(unsigned precision) {
		int rc;
		int delay = getPrecisionDelay(precision);;
		if (delay < 0)
			return(delay);
		rc = writeByte(CMD_ADC_CONV_PRES+precision
			);
		if (rc == 0)
			_lastCv=CMD_ADC_CONV_PRES;		// remember we did this.
		return(rc);
	}

	/**
	 * @brief read the ADC value and convert and store it in this object.
	 * 
	 * @return int 
	 */
	virtual int readAdc() = 0;

	/**
	 * @brief Get the Last Pressure that and conversion was done for.
	 * 
	 * @return int 
	 */ 
	int getLastPressure() {
		return(_pressure);
	}

	/**
	 * @brief Get the Last Temperature value read for this sensor.
	 * 
	 * @return int 
	 */
	int getLastTemperature() {
		return(_temperature);
	}

	// TEST functions for photon

	/**
	 * @brief Set the Test Pressure for loop back test
	 * 
	 * @param pressure -- pressure to set
	 * @return int  0 if OK, otherwise PI_BAD_HANDLE, PI_BAD_PARAM, or PI_I2C_WRITE_FAILED.
	 */
	int setTestPressure(unsigned pressure) {
		uint8_t buf[3];
		buf[0] = (pressure>>16);
		buf[1] = (pressure>>8);
		buf[2] = pressure;
		int rc = writeBlockData(CMD_ADC_TEST_PRES,buf, sizeof(buf));
		return (rc);
	}

	/**
	 * @brief Get the Test Pressure setting
	 * 
	 * @return int 
	 */
	int getTestPressure() {
		uint8_t buf[3];
		int rc = readBlockData(CMD_ADC_TEST_PRES, buf, sizeof(buf));
		if (rc > 0) {
			rc = (buf[0]<<16) + (buf[1]<<8) + buf[2];
		}
		return (rc);
	}

	/**
	 * @brief Set the Test Temperature 
	 * 
	 * @param temperature 
	 * @return int 
	 */
	int setTestTemperature(unsigned temperature) {
		uint8_t buf[3];
		buf[0] = (_pressure>>16);
		buf[1] = (_pressure>>8);
		buf[2] = _pressure;
		int rc = writeBlockData(CMD_ADC_TEST_TEMP,buf, sizeof(buf));
		return (rc);
	}
	/**
	 * @brief Get the Test Temperature value.
	 * 
	 * @return int 
	 */
	int getTestTemperature() {
		uint8_t buf[3];
		int rc = readBlockData(CMD_ADC_TEST_TEMP, buf, sizeof(buf));
		if (rc > 0) {
			rc = (buf[0]<<16) + (buf[1]<<8) + buf[2];
		}
		return (rc);
	}
	

private:
};

class I2cMS5837 : public I2cMSxxx
{
public:
	I2cMS5837() = delete;
	I2cMS5837(unsigned bus, unsigned address)
		: I2cMSxxx(bus, address) {
		memset(_prom, 0, sizeof(_prom));
	}

	/**
	 * @brief read the ADC value and convert and store it in this object.
	 * 
	 * @return int 
	 */
	int readAdc() {
		switch (_lastCv) {
		case CMD_ADC_CONV_PRES:		// todo, do the conversion here.
			break;
		case CMD_ADC_CONV_TEMP:		// todo, do the conversion here.
			break;
		}
		return (0);		// tbd, do the actual conversion...
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
		switch (precision)
		{
		case ADC_256 : delay =   560; break;
		case ADC_512 : delay =  1100; break;
		case ADC_1024: delay =  2170; break;
		case ADC_2048: delay =  4320; break;
		case ADC_4096: delay =  8610; break;
		case ADC_8192: delay = 17200; break;
		default: delay = -1;		// unknown precision...
		}
		return (delay);
	}
private:


};

class I2cPhotonCfgDev : public I2cGenericDev 
{
public:
	I2cPhotonCfgDev() = delete;
	I2cPhotonCfgDev(unsigned bus, unsigned address)
		: I2cGenericDev(bus, address) {}

	/**
	 * @brief Get the Version word
	 * 
	 * @return int -- version word, or error code if error
	 */
	int getVersion() {
		uint8_t buf[2];
		int rc = readBlockData(VERSION, buf, sizeof(buf));
		if (rc > 0) {
			rc = (buf[0]<<8) + buf[1];
		}
		return (rc);
	}
	/**
	 * @brief Get the Siminterval from the slave i2c device.
	 * 
	 * @return int 
	 */
	int getSiminterval() {
		uint8_t buf[2];
		int rc = readBlockData(SIMINTERVAL, buf, sizeof(buf));
		if (rc > 0) {
			rc = (buf[0]<<8) + buf[1];
		}
		return (rc);
	}
	int setSimInterval(unsigned simInterval) {
		uint8_t buf[2];
		buf[0] = (simInterval>>8);
		buf[1] = simInterval;
		int rc = writeBlockData(SIMINTERVAL, buf, sizeof(buf));
		return (rc);
	}

protected:
	enum {
		VERSION = 0, 
		        // return with the version number of this simulation (replies with 2 bytes, major and minor)
		SIMINTERVAL = 1     // set the sim interval in milliseconds.
	};

private:
};


int main(int argc, char *argv[])
{
	if (gpioInitialise() < 0)	{
		cout << "gpioInitialize Failed" << endl;
		return (1);
	}
	cout << "callling initI2c" << endl;
	I2cPhotonCfgDev photonCfg(1, PHOTON_CFG_I2C_DEV);
	if (!photonCfg.initI2c()) 	{
		cout << "initI2c Failed" << endl;
		return (1);
	}

	
	int version = photonCfg.getVersion();
	cout << "version=" << (version>>8) << ":" << (version&0xFF) << endl;
	
	int siminterval = photonCfg.getSiminterval();
	cout << "siminterval=" << siminterval << endl;
	
	gpioTerminate();
	
	return 0;
}