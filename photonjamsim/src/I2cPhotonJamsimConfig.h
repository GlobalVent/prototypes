
#include "I2cSlaveDevice.h"

/**
 * @brief I2cPhotonJamsimConfig -- photonjamsim slave config i2c device
 * 
 */
class I2cPhotonJamsimConfig : public I2cSlaveDevice
{
public:
    I2cPhotonJamsimConfig() = delete;
    /**
     * @brief Construct a new I2cSlaveDevice object
     * 
     * @param devAddr -- device address for this device.
     */
    I2cPhotonJamsimConfig(unsigned devAddr) :
        I2cSlaveDevice(devAddr) {};

    /**
     * @brief startTransaction -- this call happens after a start event AND
     *           and the the caller has received an address matching the 
     *           devAddr
     * @param -- read -- true of this is a read transaction, false if it is a write.
     */
    virtual void start(bool read);

    /**
     * @brief stop event received AFTER receiving a start event...
     * 
     * @param -- read -- true of this is a read transaction, false if it is a write.
     */
    virtual void stop(bool read);

    /**
     * @brief read the next byte of data
     *        the remote computer is asking for more data so deliver it
     *        if this has no more to send then return zeros.
     * 
     * 
     * @return uint8_t 
     */
    virtual uint8_t read();
    /**
     * @brief write data to the device. (one byte at a time.)
     * 
     * @param data -- 1 byte data written to the device
     */
    virtual void write(uint8_t data);

protected:
private:
};
