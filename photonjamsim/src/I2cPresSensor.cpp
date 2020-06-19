


#include "I2cPresSensor.h"


/**
 * @brief startTransaction -- this call happens after a start event AND
 *           and the the caller has received an address matching the 
 *           devAddr
 * @param -- _rw -- read/write_
 * 
 */
void I2cPresSensor::start(unsigned _rw) {
    stop(_rw);                         // if we have a command in progress and get another start it implies a stop.
    I2cSlaveDevice::start(_rw);
}

/**
 * @brief stop event received AFTER receiving a start event...
 * 
 * @param -- _rw -- read/write_
 */
void I2cPresSensor::stop(unsigned _rw) {
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
 * @return uint8_t -- return the read data
 *                 return false when we read more than the device 
 *                 has in the register associated with the command
 *                 written.
 */
uint8_t I2cPresSensor::read() {
    #if 0
    bool rc = true;
    switch (_command) {     // data goes out in network byte order
        case 0:                    
            // todo, deal with commands here.
            break;
    }
    _sendByteIdx++;
    return(rc);
    #endif
    return(0);
}
/**
 * @brief write data to the device. (one byte at a time.)
 * 
 * @param data -- 1 byte data written to the device
 */
void I2cPresSensor::write(uint8_t data) {
    #if 0
    if (_recvByteCount == 0)        // first byte, pick up the command.
        _command=data;
    else {
        if (_recvByteCount < sizeof(_recvData)) 
            _recvData[_recvByteCount] = data;
    }
    _recvByteCount++;
    #endif
}


