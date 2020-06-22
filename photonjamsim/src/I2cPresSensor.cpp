

#include <string.h>
#include "I2cPresSensor.h"
#include "crc4.h"


I2cPresSensor::I2cPresSensor(unsigned devAddr) :
    I2cSlaveDevice(devAddr),
    _temperature(293),
    _pressure(1000) {
        _prom[0] = 0;
        _prom[1] = 46372;
        _prom[2] = 43981;
        _prom[3] = 29059;
        _prom[4] = 27842;
        _prom[5] = 31553;
        _prom[6] = 28165;
        _prom[7] = 0;
        _prom[7] = crc4(_prom);
}

void I2cPresSensor::clearSendData() 
{
    memset(_sendData, 0, sizeof(_sendData));
}
/**
 * @brief stop event received AFTER receiving a start event...
 * 
 * @param -- _rw -- read/write_
 */
void I2cPresSensor::stop(unsigned rw) {
    if (rw)            // nothing to do on a read....
        return;
    if (_recvByteCount == 0)
        return;
    _regAddr=_recvData[0];   // remember the last write.
    
    switch(_regAddr) {
        case CMD_RESET: 
            clearSendData();
            break;
        case CMD_ADC_READ:
            break;
        case CMD_SET_TEMP:
            if (_recvByteCount >= 3)
                _temperature = (_recvData[1] << 8) + _recvData[2];
            _sendByteCount=0;                                       
            _sendData[_sendByteCount++] = (_temperature>>16) & 0xFF;
            _sendData[_sendByteCount++] = (_temperature>>8) & 0xFF;
            _sendData[_sendByteCount++] = _temperature & 0xFF;
            break;
        case CMD_SET_PRES:
            if (_recvByteCount >= 3)
                _pressure = (_recvData[1] << 8) + _recvData[2];
            _sendByteCount=0;                                       
            _sendData[_sendByteCount++] = (_pressure>>16) & 0xFF;
            _sendData[_sendByteCount++] = (_pressure>>8) & 0xFF;
            _sendData[_sendByteCount++] = _pressure & 0xFF;
            break;
        default: {
            switch (_regAddr >> 4) {
                case CMD_CV_D1:
                    clearSendData();        // clear now, time delay to get the data.
                    break;
                case CMD_CV_D2:
                    clearSendData();        // clear now, time delay to get the data.
                    break;
                case CMD_PROM_RD: {
                    unsigned paddr = (_regAddr & 0xf)>>1;
                    uint16_t pd = _prom[paddr];
                    _sendByteCount = 0;
                    _sendData[_sendByteCount++] = (pd>>8) & 0xFF;
                    _sendData[_sendByteCount++] = pd & 0xFF;
                    break;
                }
            }
            break;
        }

    }
}

