


#include "I2cPhotonJamsimConfig.h"


/**
 * @brief startTransaction -- this call happens after a start event AND
 *           and the the caller has received an address matching the 
 *           devAddr
 * @param -- _rw -- read/write_
 * 
 */
void I2cPhotonJamsimConfig::start(unsigned _rw) {
    stop(_rw);                         // if we have a command in progress and get another start it implies a stop.
    I2cSlaveDevice::start(_rw);
}

/**
 * @brief stop event received AFTER receiving a start event...
 * 
 * @param -- _rw -- read/write_
 */
void I2cPhotonJamsimConfig::stop(unsigned _rw) {
    switch (_command) {
        case 1:             // sim interval
            if (_recvData.size() >= 2)
                _simInterval = (_recvData[0] << 1) + _recvData[0];
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
bool I2cPhotonJamsimConfig::read(uint8_t &data) {
    bool rc = true;
    switch (_command) {     // data goes out in network byte order
        case 0:                     // get version number.
            if (_byteCount < 2) 
                data = (_version >> _byteCount) & 0xFF;
            else
                data = 0, rc=false;
            break;
        case 1:             // sim interval
            if (_byteCount < 2) 
                data = (_simInterval >> _byteCount) & 0xFF;
            else
                data = 0, rc=false;
            break;
    }
    _byteCount++;
    return(rc);
}
/**
 * @brief write data to the device. (one byte at a time.)
 * 
 * @param data -- 1 byte data written to the device
 */
void I2cPhotonJamsimConfig::write(uint8_t data) {
    if (_byteCount == 0)        // first byte, pick up the command.
        _command=data;
    else {
        if (_byteCount < 2)
            _recvData.push_back(data);
    }
    _byteCount++;
}


