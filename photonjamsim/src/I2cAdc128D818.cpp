


#include "I2cAdc128D818.h"


/**
 * @brief stop event received AFTER receiving a start event...
 * 
 * @param -- _rw -- read/write_
 */
void I2cAdc128D818::stop(unsigned rw) {
    if (rw)            // nothing to do on a read....
        return;
    if (_recvByteCount == 0)
        return;
    _regAddr=_recvData[0];   // remember the last write.
    if ((_regAddr >= REG_CHANNEL_READ) && (_regAddr <= REG_CHANNEL_READ+7)) {
        _sendByteCount=0;
        unsigned d = _regChannelRead[_regAddr-REG_CHANNEL_READ];
        _sendData[_sendByteCount++] = (d >> 8) & 0xFF ;
        _sendData[_sendByteCount++] = (d >> 0) & 0xFF ;
        return;
    }
    if ((_regAddr >= REG_LIMIT) && (_regAddr <= REG_LIMIT+7)) {
        _sendByteCount=0;
        unsigned d = _regLimit[_regAddr-REG_LIMIT];
        _sendData[_sendByteCount++] = (d >> 8) & 0xFF ;
        _sendData[_sendByteCount++] = (d >> 0) & 0xFF ;
        return;
    }
    // fake loopback register
    if ((_regAddr >= REG_CHANNEL_WRITE) && (_regAddr <= REG_CHANNEL_WRITE+7)) {
        unsigned idx = _regAddr-REG_CHANNEL_WRITE;
        if (_recvByteCount >= 2)
            _regChannelRead[idx] = (_recvData[1] << 8) + _recvData[2];
        _sendByteCount=0;
        unsigned d = _regChannelRead[idx];
        _sendData[_sendByteCount++] = (d >> 8) & 0xFF ;
        _sendData[_sendByteCount++] = (d >> 0) & 0xFF ;
        return;
    }

    // the rest are individual byte read/writes....
    switch(_regAddr) {
        case REG_CFG:                      // configuration register
            _sendByteCount=0;
            _sendData[_sendByteCount++] = _regConfig;
            break;
        case REG_INTERRUPT_STATUS:         // interrupt status register.
            _sendByteCount=0;
            _sendData[_sendByteCount++] = _regInterruptStatus;
            break;
        case REG_INTERRUPT_MASK:           // interrupt mask register
            _sendByteCount=0;
            _sendData[_sendByteCount++] = _regInterrruptMask;
            break;
        case REG_CONVERSION_RATE:          // conversion rate register
            _sendByteCount=0;
            _sendData[_sendByteCount++] = _regConversionRate;
            break;
        case REG_CHANNEL_DISABLE:           // channel disable reigster
            _sendByteCount=0;
            _sendData[_sendByteCount++] = _regChannelDisable;
            break;
        case REG_ONE_SHOT:                  // one shot register
            break;                          // write only register, nothing to do here...
                                            // as we don't simulate this behavior
        case REG_DEEP_SHUTDOWN:             // deep shutdown register
            _sendByteCount=0;
            _sendData[_sendByteCount++] = _regDeepShutdown;
            break;
        case REG_ADVANCED_CONFIG:           // advanced configuration register
            _sendByteCount=0;
            _sendData[_sendByteCount++] = _regAdvancedConfig;       
            break;
        case REG_BUSY_STATUS:               // busy status register.
            _sendByteCount=0;
            _sendData[_sendByteCount++] = _regBusyStatus;
            break;
        case REG_MFG_ID:                    // manufacture id register
            _sendByteCount=0;
            _sendData[_sendByteCount++] = _regMfgId;
            break;
        case REG_REVISION_ID:               // revision id.
            _sendByteCount=0;
            _sendData[_sendByteCount++] = _regRevId;
            break;
        default:
            break;      // ignore everything else/.
    }

}

