#ifndef __I2C_SLAVE_CTL_H__
#define __I2C_SLAVE_CTL_H__

#include <string>
#include <map>
#include "I2cSlaveDevice.h"
#include "JamsimDbgPrint.h"


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

    void setDbgPrint(JamsimDbgPrint *dbgPrint) {
        _dbgPrint = dbgPrint;
    }
    std::string lastError() {
        return(_lastError);
    }
    /**
     * @brief perform an i2c sample of the signals
     *        any class calls to the i2c slave devices happen in 
     *        this step function.
     * 
     */
    void sampleIO(double now);

    /**
     * @brief register the i2c device with the controller.
     * 
     * @param dev pointer to the device to regsiter.
     * @return true if successful fasle id already in use.
     */
    bool registerI2cDevice(I2cSlaveDevice *dev);
    
    
protected:
    void goToStateReady(unsigned from);
    void goToStateAddr(unsigned from);
    void goToStateAckAddr(unsigned from);
    void goToStateWr(unsigned from);
    void goToStateRd(unsigned from);
    void goToStateAck(unsigned from);
private:
    unsigned _sclGpio;
    unsigned _sdaGpio;
    std::ostream *_log;
    std::string _lastError;
    JamsimDbgPrint *_dbgPrint;
    std::map<unsigned,I2cSlaveDevicePtr> _registeredDevs;     // devices registerd to this slave controller.
    unsigned _i2cLastGpio;                // sample bits SCL/SDA
    unsigned _i2cState;
    I2cSlaveDevicePtr *_currDev;        // currently addressed device

    bool     _rw;                          // substates for the state machine below
    bool     _bitCount;                         
    uint8_t  _currByte;                 // current byte under assembly

    // state names and implementation comes from here.
    // https://www.digikey.com/eewiki/pages/viewpage.action?pageId=10125324
    //   modified as start and stop are actually events, not states
    enum {
        I2C_STATE_READY     = 0,
        I2C_STATE_ADDR      = 1,
        I2C_STATE_ACK_ADDR  = 2,
        I2C_STATE_WR        = 3,
        I2C_STATE_ACK       = 4,
        I2C_STATE_RD        = 5
    };

    enum {
        I2C_EVENT_INVAL,
        I2C_EVENT_START,
        I2C_EVENT_STOP,
        I2C_EVENT_SCL_DOWN,
        I2C_EVENT_SCL_UP
    };


};


#endif