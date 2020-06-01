#ifndef __JAMHW_H__
#define __JAMHW_H__

#include <stdint.h>
#include <ostream>

/**
 * @brief JamHw
 * 
 * Hardware interface class responsible for all reads and writes
 *          for dealing with hardware.
 * 
 * This hardware layer has two components.
 *    a synchronous I2c read sequence that will read the i2c devices and store the data
 *    in the class for future retervial
 *
 *    and an asynchronous interface for the CPLD serial communications
 *  
 */
class JamHw {
public:	
	JamHw() {};
	virtual ~JamHw() {}

	/**
	 * @brief setDebugLevel
	 * @details set the debug level to use for debugging 
	 *          must have done the setLogStream below
	 * 
	 * @param level  -- debug level to set, 0, no debug data. 1,2,3... increasing verbosity
	 */

	virtual void setDebugLevel(unsigned level);
	/**
	 * @brief setLogStream 
	 * @details set a logStream device to help with debugging...
	 * 
	 * @param ostr ostream object to send log data to.
	 */
	virtual void setLogStream(std::ostream *ostr); // set a log os stream device.  some place to send log info.

    /**
     * @brief initiate a read of all hardware i2c and serial data.
     *        the I2c reads will be done synchronously and are expected
     *        to take about 10ms to complete and will attempt to minimize 
     *        jitter.
     *      
     *        the hardware read cycle should read the following sensors:
     *          * Pres pressure sensor
     *          * Psys pressure sensor
     *          * o2Adc -- the o2 adc sensor.
     *          * CPLD serial port if data is available (read asynchronously)
     *          *    temperature sensors in the pressure sensors should be read every second and
     *               the data kept in this class.  NOTE: we should strive to not have the temperature read
     *               result in a multi millisecond delay if possible, reading that sensor, if possible
     *               should be combined with reading of the other sensor data so any wait periods overlap.
     */
    virtual void readHwData();

    /**
     * @brief getPsys -- read the pSys sensor.
     *    this should get the pRes data from the last call to readHwData.
     * 
     * @return double pSys sensor reading in absolute bars
     *                     
     */
    virtual double getPsys();

    /**
     * @brief getPres -- read the pRes sensor.
     *    this should get the pRes data from the last call to readHwData.
     * 
     * @return double pRes sensor reading in absolute bars
     *                     
     */
    virtual double getPres();

    /**
     * @brief getPO2 -- get the pO2 reading.
     * 
     * @return double 
     */
    virtual double getPO2();

	/**
	 * @brief set Valvex to open or close.
	 *            This will result in a direct gpio write value to the pigpio library.
	 * 
	 * @param open  -- true to open, false to close.
	 */
	virtual void setValveA(bool open);
	virtual void setValveB(bool open);
	virtual void setValveC(bool open);
	virtual void setValveD(bool open);

	/**
	 * @brief Get the ValveX current setting.
	 *    this just reads back the value previously set.
	 * @return true -- valve is open
	 * @return false -- valve is closed.
	 */
	virtual bool getValveA();
	virtual bool getValveB();
	virtual bool getValveC();
	virtual bool getValveD();

	/**
	 * @brief Set the Valve gpio pins
	 * 
	 * @param valveA -- gpio pin for valve a
	 * @param valveB -- gpio pin for valve b
	 * @param valveC -- gpio pin for valve c
	 * @param valveD -- gpio pin for valve d
	 */
	virtual void setValveGpio(unsigned valveA,
	                          unsigned valveB,
							  unsigned valveC,
							  unsigned valveD);

	/**
	 * @brief setI2cAddress -- set alternate i2c addresses for the i2c devices.
	 *          this is used for testing with the photon supplying the i2c data for
	 *          the data, this should allow a complete simulation of the system 
	 *          with the photon supplying a model.
	 * 
	 * @param pSysI2c 
	 * @param pResI2c 
	 * @param pO2I2c 
	 */
	virtual void setI2cAddress(unsigned pSysI2c,
								 unsigned pResI2c,
								 unsigned pO2I2c);


	/**
	 * @brief write out the cpld keep alive sequence.
	 *        this will write out the keep alive sequence asynchronously
	 *        if the data cannot be sent right now, then buffer it 
	 *        and send it on the next readHwData call.  Do not block
	 *        during this call.
	 *        if more than 5 calls are made and the data is still not
	 *        left the building then return false and ignore the data.
	 * 
	 * @param data -- data bytes to write.
	 * @param len -- length of the data to write.
	 * 
	 */
	virtual bool writeCpldKeepalive(const uint8_t *data, unsigned len);




protected:
private:
};

#endif

