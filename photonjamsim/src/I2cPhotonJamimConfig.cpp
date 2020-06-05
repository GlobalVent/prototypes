


#include "I2cPhotonJamsimConfig.h"


/**
 * @brief startTransaction -- this call happens after a start event AND
 *           and the the caller has received an address matching the 
 *           devAddr
 * @param -- read -- true of this is a read transaction, false if it is a write.
 * 
 */
void I2cPhotonJamsimConfig::start(bool read) {
    I2cSlaveDevice::start(read);
}

/**
 * @brief stop event received AFTER receiving a start event...
 * 
 * @param -- read -- true of this is a read transaction, false if it is a write.
 */
void I2cPhotonJamsimConfig::stop(bool read) {
    I2cSlaveDevice::stop(read);

    // we got the stop command, if this transaction was a write
    // then act on the data.

}

/**
 * @brief read the next byte of data
 *        the remote computer is asking for more data so deliver it
 *        if this has no more to send then return zeros.
 * 
 * 
 * @return uint8_t 
 */
uint8_t I2cPhotonJamsimConfig::read() {
    return('a'+_byteCount);
    I2cSlaveDevice::read();
}
/**
 * @brief write data to the device. (one byte at a time.)
 * 
 * @param data -- 1 byte data written to the device
 */
void I2cPhotonJamsimConfig::write(uint8_t data) {

    I2cSlaveDevice::write(data);
}


