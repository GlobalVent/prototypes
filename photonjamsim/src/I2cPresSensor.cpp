


#include "I2cPresSensor.h"



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

