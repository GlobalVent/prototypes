#ifndef __I2C_SLAVE_DEV_H__
#define __I2C_SLAVE_DEV_H__

#include <vector>
#include <stdint.h>
#include <string.h>
#include "JamsimDbgPrint.h"

class I2cSlaveDevice
{
public:
    I2cSlaveDevice() = delete;
    /**
     * @brief Construct a new I2cSlaveDevice object
     * 
     * @param devAddr -- device address for this device.
     */
    I2cSlaveDevice(unsigned devAddr) :
        _dbgPrint(nullptr),
        _devAddr(devAddr),
        _recvByteCount(0),
        _sendByteCount(0),
        _sendByteIdx(0),
        _regAddr(0xFF)    {
            memset(_sendData, 0xFF, sizeof(_sendData));
            memset(_recvData, 0xFF, sizeof(_recvData));
        }     // current index of being sent.
    virtual ~I2cSlaveDevice() {}
    /**
     * @brief Get the Dev address object
     * 
     * @return device address
     */
    unsigned getDevAddr() { return _devAddr;};

    void setDbgPrint(JamsimDbgPrint *dbgPrint) {
        _dbgPrint = dbgPrint;
    }

    /**
     * @brief startTransaction -- this call happens after a start event AND
     *           and the the caller has received an address matching the 
     *           devAddr
     * @param -- rw bit from transaction 1=read, 0 write
     */
    virtual void start(unsigned rw) {
        stop(rw);                         // if we have a command in progress and get another start it implies a stop.
        _recvByteCount=0;
        _sendByteIdx=0;
    }

    /**
     * @brief stop event received AFTER receiving a start event...
     * 
     * @param -- rw bit from transaction 1=read, 0 write
     */
    virtual void stop(unsigned rw) {};

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
    virtual uint8_t read(){
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
    virtual void write(uint8_t data){
        if (_recvByteCount < sizeof(_recvData)) 
            _recvData[_recvByteCount++] = data;
    }
protected:
    JamsimDbgPrint *_dbgPrint;
    unsigned _devAddr;
    unsigned _recvByteCount;
    unsigned _sendByteCount;
    unsigned _sendByteIdx;
    uint8_t  _regAddr;          // last register address received.
    uint8_t _sendData[4];
    uint8_t _recvData[4];

    void clearSendData() 
    {
        memset(_sendData, 0, sizeof(_sendData));
    }

private:
};


typedef I2cSlaveDevice *I2cSlaveDevicePtr;
#endif