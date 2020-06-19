


#include "I2cJamsimConfig.h"


/**
 * @brief startTransaction -- this call happens after a start event AND
 *           and the the caller has received an address matching the 
 *           devAddr
 * @param -- _rw -- read/write_
 * 
 */
void I2cJamsimConfig::start(unsigned _rw) {
    stop(_rw);                         // if we have a command in progress and get another start it implies a stop.
    I2cSlaveDevice::start(_rw);
}

/**
 * @brief stop event received AFTER receiving a start event...
 * 
 * @param -- _rw -- read/write_
 */
void I2cJamsimConfig::stop(unsigned _rw) {
    if (_recvByteCount == 0)
        return;
    if (_rw)       // nothing to do on a read cycle...
        return;

    switch (_recvData[0]) {
        case VERSION:               
            if (_rw) {
                _sendByteCount=0;
                _sendData[_sendByteCount++] = (_version>>8) & 0xFF;
                _sendData[_sendByteCount++] = _version & 0xFF;
            }
            break;
        case SIMINTERVAL:           
            if (_rw) {
                _sendByteCount=0;
                _sendData[_sendByteCount++] = (_simInterval>>8) & 0xFF;
                _sendData[_sendByteCount++] = _simInterval & 0xFF;
            } 
            else {
                if (_recvByteCount >= 3)
                    _simInterval = (_recvData[1] << 8) + _recvData[2];
            }
            break;
        default:
            memset(_sendData, 0xAA, sizeof(_sendData));   // give us a pattern, 
            break;  

    }
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
uint8_t I2cJamsimConfig::read() {
    uint8_t data;
    if (_sendByteIdx < sizeof(_sendData))
        data = _sendData[_sendByteIdx++];
    else
        data = 0;
    return(data);
}
/**
 * @brief write data to the device. (one byte at a time.)
 * 
 * @param data -- 1 byte data written to the device
 */
void I2cJamsimConfig::write(uint8_t data) {
    if (_recvByteCount < sizeof(_recvData)) 
        _recvData[_recvByteCount++];
}


