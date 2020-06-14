#ifndef __I2C_PSYS_SENSOR_H__
#define __I2C_PSYS_SENSOR_H__

#include "I2cSlaveDevice.h"

/**
 * @brief I2cPsysSensor -- photonjamsim slave config i2c device
 * 
 */
class I2cPsysSensor : public I2cSlaveDevice
{
public:
    I2cPsysSensor() = delete;
    /**
     * @brief Construct a new I2cSlaveDevice object
     * 
     * @param devAddr -- device address for this device.
     */
    I2cPsysSensor(unsigned devAddr) :
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


protected:
    int      _command;
private:
};
#endif
