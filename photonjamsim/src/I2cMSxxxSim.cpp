#include <string.h>
#include "I2cMSxxxSim.h"


I2cMSxxxSim::I2cMSxxxSim(unsigned devAddr) :
    I2cSlaveDevice(devAddr),
    _lastConv(0),
    _temperature(293),
    _pressure(1000),
    _temperature_raw(0),
    _pressure_raw(0),
    _OFF(0),
    _SENS(1) {
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

/**
 * @brief crc4
 *        calculate the crc 4 value for the MSxxx family of pressure sensors.
 *  taken from: https://www.amsys-sensor.eu/sheets/amsys.fr.an520_e.pdf
 * @param n_prom 
 * @return c4c of prom
 */
uint16_t I2cMSxxxSim::crc4(uint16_t n_prom[])
{

    int cnt; // simple counter
    unsigned int n_rem; // crc reminder
    unsigned int crc_read; // original value of the crc
    unsigned char n_bit;
    n_rem = 0x00;
    crc_read=n_prom[7]; //save read CRC
    n_prom[7]=(0xFF00 & (n_prom[7])); //CRC byte is replaced by 0
    for (cnt = 0; cnt < 16; cnt++) // operation is performed on bytes
    { // choose LSB or MSB
        if (cnt%2==1) 
            n_rem ^= (unsigned short) ((n_prom[cnt>>1]) & 0x00FF);
        else 
            n_rem ^= (unsigned short) (n_prom[cnt>>1]>>8);
        for (n_bit = 8; n_bit > 0; n_bit--)         {
            if (n_rem & (0x8000))         {
                n_rem = (n_rem << 1) ^ 0x3000;
            } else         {
                n_rem = (n_rem << 1);
            }
        }
    }
    n_rem= (0x000F & (n_rem >> 12)); // // final 4-bit reminder is CRC code
    n_prom[7]=crc_read; // restore the crc_read to its original place
    return (n_rem ^ 0x00);
}

/**
 * @brief doConversion -- perform a temperauture or pressure conversion.
 * 
 * @param -- conv -- convesion to do...
 */
void I2cMSxxxSim::doConversion(unsigned conv) 
{
    int delay = getPrecisionDelay(conv & 0xF);
    if (delay < 0) 
        return;         // invalid delay, just ignore it...
    switch (conv & 0xF0) {
        case CMD_CV_D1:
            _temperature_raw = convTemperature(_temperature);        
            break;
        case CMD_CV_D2:
            _pressure_raw = convPressure(_pressure);
            break;
        default:
            return;
    }
    _lastConv = conv & 1;       // remember which one we did...
    _timeConv = _jtime.now() + (delay*1e-6);

}
/**
 * @brief Get the Precision value
 * 
 * @param precision -- precision to look up the delay for
 * @return unsigned -- delay in us...
 */
int I2cMSxxxSim::getPrecisionDelay(unsigned precision) 
{
    int delay;
    switch (precision)
    {
    case CMD_OSR_256 : delay =   600; break;
    case CMD_OSR_512 : delay =  1170; break;
    case CMD_OSR_1024: delay =  2280; break;
    case CMD_OSR_2048: delay =  4540; break;
    case CMD_OSR_4096: delay =  9040; break;
    default: delay = -1;		// unknown precision...
    }
    return (delay);
}

/**
 * @brief stop event received AFTER receiving a start event...
 * 
 * @param -- _rw -- read/write_
 */
void I2cMSxxxSim::stop(unsigned rw) {
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
            if (_jtime.now() >= _timeConv) {        // time to release the data?
                switch (_lastConv) {
                    case CMD_CV_D1:
                        _sendByteCount=0;                                       
                        _sendData[_sendByteCount++] = (_temperature_raw>>16) & 0xFF;
                        _sendData[_sendByteCount++] = (_temperature_raw>>8) & 0xFF;
                        _sendData[_sendByteCount++] = _temperature_raw & 0xFF;
                        break;
                    case CMD_CV_D2:
                        _sendByteCount=0;                                       
                        _sendData[_sendByteCount++] = (_pressure_raw>>16) & 0xFF;
                        _sendData[_sendByteCount++] = (_pressure_raw>>8) & 0xFF;
                        _sendData[_sendByteCount++] = _pressure_raw & 0xFF;
                        break;
                }
            }
            else {
                clearSendData();
            }
            break;
        case CMD_SET_TEMP:
            if (_recvByteCount >= 3)
                _temperature = (_recvData[1] << 16) + (_recvData[2] << 8) + _recvData[3];
            _sendByteCount=0;                                       
            _sendData[_sendByteCount++] = (_temperature>>16) & 0xFF;
            _sendData[_sendByteCount++] = (_temperature>>8) & 0xFF;
            _sendData[_sendByteCount++] = _temperature & 0xFF;
            break;
        case CMD_SET_PRES:
            if (_recvByteCount >= 3)
                _pressure = (_recvData[1] << 16) + (_recvData[2] << 8) + _recvData[3];;
            _sendByteCount=0;                                       
            _sendData[_sendByteCount++] = (_pressure>>16) & 0xFF;
            _sendData[_sendByteCount++] = (_pressure>>8) & 0xFF;
            _sendData[_sendByteCount++] = _pressure & 0xFF;
            break;
        default: {
            switch (_regAddr & 0xF0) {
                case CMD_CV_D1:
                    clearSendData();        // clear now, time delay to get the data.
                    doConversion(_regAddr);
                    break;
                case CMD_CV_D2:
                    clearSendData();        // clear now, time delay to get the data.
                    doConversion(_regAddr);
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

