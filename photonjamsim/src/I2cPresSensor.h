
#ifndef __I2C_PRES_SENSOR_H__
#define __I2C_PRES_SENSOR_H__

#include "I2cSlaveDevice.h"

/**
 * @brief I2cPresSensor -- photonjamsim slave config i2c device
 * 
 */
class I2cPresSensor : public I2cSlaveDevice
{
public:
    I2cPresSensor() = delete;
    /**
     * @brief Construct a new I2cSlaveDevice object
     * 
     * @param devAddr -- device address for this device.
     */
    I2cPresSensor(unsigned devAddr) :
        I2cSlaveDevice(devAddr),
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
     * @return uint8_t -- return the read data
     *                 return false when we read more than the device 
     *                 has in the register associated with the command
     *                 written.
     */
    virtual uint8_t read();
    /**
     * @brief write data to the device. (one byte at a time.)
     * 
     * @param data -- 1 byte data written to the device
     */
    virtual void write(uint8_t data);


protected:
    int      _command;
private:
};
#endif
