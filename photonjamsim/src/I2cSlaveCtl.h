#ifndef __I2C_SLAVE_CTL_H__
#define __I2C_SLAVE_CTL_H__

#include <string>
#include <map>
#include "I2cSlaveDevice.h"


/**
 * @brief I2cSlaveCtl -- slave control class for i2c bit banger.
 * 
 */
class I2cSlaveCtl
{
public:
    I2cSlaveCtl() = delete;
    I2cSlaveCtl(int sdlGpio, int sdaGpio);
    virtual ~I2cSlaveCtl();

    std::string lastError() {
        return(_lastError);
    }
    /**
     * @brief perform an i2c sample of the signals
     *        any class calls to the i2c slave devices happen in 
     *        this step function.
     * 
     */
    void sampleIO();

    /**
     * @brief register the i2c device with the controller.
     * 
     * @param dev pointer to the device to regsiter.
     * @return true if successful fasle id already in use.
     */
    bool registerI2cDevice(I2cSlaveDevice *dev);
    
    
protected:
private:
    unsigned _sdlGpio;
    unsigned _sdaGpio;
    std::string _lastError;
    std::map<unsigned,I2cSlaveDevicePtr> _registeredDevs;     // devices registerd to this slave controller.
    unsigned _i2cLastSample;                // sample bits SCL/SDA
    unsigned _i2cState;
    bool     _read;                         // this is a read operation
};


#endif