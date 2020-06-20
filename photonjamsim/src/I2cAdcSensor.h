#ifndef __I2C_PRES_SENSOR_H__
#define __I2C_PRES_SENSOR_H__

#include "I2cSlaveDevice.h"

/**
 * @brief I2cAdcSensor -- photonjamsim slave config i2c device
 * 
 */
class I2cAdcSensor : public I2cSlaveDevice
{
public:
    I2cAdcSensor() = delete;
    /**
     * @brief Construct a new I2cSlaveDevice object
     * 
     * @param devAddr -- device address for this device.
     */
    I2cAdcSensor(unsigned devAddr) :
        I2cSlaveDevice(devAddr),
        _command(-1) {};


    /**
     * @brief stop event received AFTER receiving a start event...
     * 
     * @param -- _rw -- read/write_
     */
    virtual void stop(unsigned _rw);

protected:
    int      _command;
private:
};
#endif
