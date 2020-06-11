
#ifndef __I2C_JAMSIM_CONFIG_H__
#define __I2C_JAMSIM_CONFIG_H__

#include "I2cSlaveDevice.h"

/**
 * @brief I2cJamsimConfig -- photonjamsim slave config i2c device
 * 
 */
class I2cJamsimConfig : public I2cSlaveDevice
{
public:
    I2cJamsimConfig() = delete;
    /**
     * @brief Construct a new I2cSlaveDevice object
     * 
     * @param devAddr -- device address for this device.
     */
    I2cJamsimConfig(unsigned devAddr) :
        I2cSlaveDevice(devAddr),
        _version(1),
        _simInterval(10),
        _command(-1) {};

    /**
     * @brief startTransaction -- this call happens after a start event AND
     *           and the the caller has received an address matching the 
     *           devAddr
     * @param -- _rw -- read/write_
     */
    virtual void start(unsigned _rw);

    /**
     * @brief stop event received AFTER receiving a start event...
     * 
     * @param -- _rw -- read/write_
     */
    virtual void stop(unsigned _rw);

        /**
     * @brief read the next byte of data
     *        the remote computer is asking for more data so deliver it
     *        if this has no more to send then return zeros.
     * 
     * @param data -- place to put next read value.
     * @return bool -- read data valid 
     *                 return false when we read more than the device 
     *                 has in the register associated with the command
     *                 written.
     */
    virtual bool read(uint8_t &data);
    /**
     * @brief write data to the device. (one byte at a time.)
     * 
     * @param data -- 1 byte data written to the device
     */
    virtual void write(uint8_t data);

    //
    // now to setup a registers to read and write
    // with address assignments.
    //
    // this device supports the following commands
    enum {
        VERSION = 0,        // return with the version number of this simulation (replies with 2 bytes, major and minor)
        SIMINTERVAL = 1     // set the sim interval in milliseconds.
    };

    /** set/getVersion
     * set or get the version string.
     * @param version -- version in the following for (major[15:8], minor[7:0])
     */
    void setVersion(unsigned version) { _version=version;}
    unsigned getVersion() { return(_version);}

    void setSimInterval(unsigned simInterval) { _simInterval=simInterval;}
    unsigned getSimInterval() { return(_simInterval);};

protected:
    unsigned _version;
    unsigned _simInterval;
    int      _command;
private:
};
#endif
