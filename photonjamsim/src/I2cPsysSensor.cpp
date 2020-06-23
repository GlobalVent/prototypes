


#include "I2cPsysSensor.h"



/**
 * @brief stop event received AFTER receiving a start event...
 * 
 * @param -- _rw -- read/write_
 */
void I2cPsysSensor::stop(unsigned _rw) {
    #if 0
    switch (_command) {
        case 1:             // sim interval
            if (_recvByteCount >= 2) {
                // todo, deal with received data here.
            }
                
            break;        

    }
    _command = -1;
    #endif
}

