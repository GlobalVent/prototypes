
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
        _version(0x100),
        _simInterval(10),
        _loopBack(0)  { };

    /**
     * @brief stop event received AFTER receiving a start event...
     * 
     * @param -- _rw -- read/write_
     */
    virtual void stop(unsigned _rw);

    //
    // now to setup a registers to read and write
    // with address assignments.
    //
    // this device supports the following commands
    enum {
        VERSION = 0,        // return with the version number of this simulation (replies with 2 bytes, major and minor)
        SIMINTERVAL = 1,     // set the sim interval in milliseconds.
        LOOPBACK = 2    // loop back test mode, don't have the simulator run the pressure readings.

    };

    /** set/getVersion
     * set or get the version string.
     * @param version -- version in the following for (major[15:8], minor[7:0])
     */
    void setVersion(unsigned version) { _version=version;}
    unsigned getVersion() { return(_version);}

    void setSimInterval(unsigned simInterval) { _simInterval=simInterval;}
    unsigned getSimInterval() { return(_simInterval);};

    void setLoopBack(unsigned loopBack) { _loopBack=loopBack; }
    unsigned getLoopBack() { return(_loopBack);}
protected:
    unsigned _version;
    unsigned _simInterval;
    unsigned _loopBack;
};
#endif
