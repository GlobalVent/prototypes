


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
void I2cJamsimConfig::stop(unsigned rw) {
    if (rw)            // nothing to do on a read....
        return;
    if (_recvByteCount == 0)
        return;


    _regAddr=_recvData[0];   // remember the last write.
    
    switch(_regAddr) {
        case VERSION:               
            _sendByteCount=0;
            _sendData[_sendByteCount++] = (_version>>8) & 0xFF;
            _sendData[_sendByteCount++] = _version & 0xFF;
            break;
        case SIMINTERVAL:           
            if (_recvByteCount >= 3)
                _simInterval = (_recvData[1] << 8) + _recvData[2];
            _sendByteCount=0;                                       
            _sendData[_sendByteCount++] = (_simInterval>>8) & 0xFF;
            _sendData[_sendByteCount++] = _simInterval & 0xFF;
            break;
        default:
            memset(_sendData, 0xAA, sizeof(_sendData));   // give us a pattern, 
            break;  
    }
}


