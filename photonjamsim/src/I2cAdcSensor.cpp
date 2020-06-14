


#include "I2cAdcSensor.h"


/**
 * @brief startTransaction -- this call happens after a start event AND
 *           and the the caller has received an address matching the 
 *           devAddr
 * @param -- _rw -- read/write_
 * 
 */
void I2cAdcSensor::start(unsigned _rw) {
    stop(_rw);                         // if we have a command in progress and get another start it implies a stop.
    I2cSlaveDevice::start(_rw);
}

/**
 * @brief stop event received AFTER receiving a start event...
 * 
 * @param -- _rw -- read/write_
 */
void I2cAdcSensor::stop(unsigned _rw) {
    switch (_command) {
        case 1:             // sim interval
            if (_recvByteCount >= 2) {
                // todo, deal with received data here.
            }
                
            break;        

    }
    _command = -1;
}

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
bool I2cAdcSensor::read(uint8_t &data) {
    #if 0
    bool rc = true;
    switch (_command) {     // data goes out in network byte order
        case 0:                    
            // todo, deal with commands here.
            break;
    }
    _recvByteCount++;
    return(rc);
    #endif
    return(false);
}
/**
 * @brief write data to the device. (one byte at a time.)
 * 
 * @param data -- 1 byte data written to the device
 */
void I2cAdcSensor::write(uint8_t data) {
    #if 0
    if (_recvByteCount == 0)        // first byte, pick up the command.
        _command=data;
    else {
        // todo, deal with  writes here..
    }
    _byteCount++;
    #endif
}


