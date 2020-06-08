#ifndef __I2C_SLAVE_DEV_H__
#define __I2C_SLAVE_DEV_H__

#include <vector>
#include <stdint.h>
//#include <sstream>

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
        //_log(nullptr),
        _devAddr(devAddr),
        _byteCount(0) {};
    /**
     * @brief Get the Dev address object
     * 
     * @return device address
     */
    unsigned getDevAddr() { return _devAddr;};

    #if 0
    /**
     * @brief Set the Log Stream object
     * 
     * @param log -- stream to set log to.
     */
    void setLogStream(std::ostringstream *log) {
        _log=log;
    }
    #endif

    /**
     * @brief startTransaction -- this call happens after a start event AND
     *           and the the caller has received an address matching the 
     *           devAddr
     * @param -- rw bit from transaction 1=read, 0 write
     */
    virtual void start(unsigned rw) {
        _byteCount = 0;
        _recvData.clear();
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
     * 
     * @return uint8_t 
     */
    virtual uint8_t read() {
      _byteCount++; 
      return(0);
    };
    /**
     * @brief write data to the device. (one byte at a time.)
     * 
     * @param data -- 1 byte data written to the device
     */
    virtual void write(uint8_t data) {
        _recvData.push_back(data);
    }
protected:
public:
    //std::ostream *_log;
    unsigned _devAddr;
    unsigned _byteCount;

    std::vector<uint8_t> _recvData;     // buffer to hold received data

};


typedef I2cSlaveDevice *I2cSlaveDevicePtr;
#endif