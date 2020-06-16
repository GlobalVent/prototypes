#ifndef __I2C_SLAVE_CTL_H__
#define __I2C_SLAVE_CTL_H__

#include <string>
#include <map>
#include <Particle.h>
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

    enum {
        I2C_EVENT_NC       = 0,       // no change, also boolean false
        I2C_EVENT_START    = 1,
        I2C_EVENT_STOP     = 2,
        I2C_EVENT_SCL_DOWN = 3,
        I2C_EVENT_SCL_UP   = 4,
        I2C_EVENT_INVAL    = 5
    };
    enum {
        I2C_STATE_READY     = 0,
        I2C_STATE_ADDR      = 1,
        I2C_STATE_ACK_ADDR  = 2,
        I2C_STATE_WR        = 3,
        I2C_STATE_ACK       = 4,
        I2C_STATE_RD        = 5,
        I2C_STATE_STOP_END  = 6     // wait for the final SCL up after a stop event.
    };

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
     * @param scl -- state of the scl line.
     * @param sda -- state of the sda line.
     * 
     */
    void sampleIO(unsigned i2cEvent, unsigned sda);

    /**
     * @brief Get the Sclgpio pin
     * 
     * @return scl gpio pin.
     */
    unsigned getSclGpio() { return(_sclGpio); }
    
    /**
     * @brief Get the SDA gpio pin
     * 
     * @return SDA gpio pin.
     */
    unsigned getSdaGpio() { return(_sdaGpio); }

    /**
     * @brief retrieve the current i2c state.
     * 
     * @return current I2c state.
     */
    unsigned getI2cState() {return(_i2cState);}

    /**
     * @brief reset the i2cState -- reset to the ready state.
     * 
     */
    void resetI2cState() { _i2cState = I2C_STATE_READY;}

    /**
     * @brief register the i2c device with the controller.
     * 
     * @param dev pointer to the device to regsiter.
     * @return true if successful fasle id already in use.
     */
    bool registerI2cDevice(I2cSlaveDevice *dev);

    /**
     * @brief Get the Last gpio seen by this device.
     * 
     * @return last gpio
     */
    unsigned getLastI2cGpio() {return(_i2cLastGpio);}
    

    /**
     * @brief decode an i2c event from the current scl,sda AND the lastGpio seen by this 
     *        controller.
     * 
     * @param scl 
     * @param sda 
     * @return unsigned 
     */
    unsigned decodeEvent(unsigned scl, unsigned sda);
    
protected:
    void gotoStateReady(unsigned from);
    void gotoStateAddr(unsigned from);
    void gotoStateAckAddr(unsigned from);
    void gotoStateWr(unsigned from);
    void gotoStateRd(unsigned from);
    void gotoStateAck(unsigned from);
    void gotoStateStopEnd(unsigned from);
    void writeSda(unsigned data) { digitalWriteFast(_sdaGpio, data); } 
    unsigned readSda() { return(digitalRead(_sdaGpio)); }
    void setSdaPinMode(PinMode mode);

    unsigned _sclGpio;
    unsigned _sdaGpio;
    unsigned _i2cLastGpio;                // sample bits SCL/SDA
    std::ostream *_log;
    std::string _lastError;
    JamsimDbgPrint *_dbgPrint;
    typedef std::map<unsigned,I2cSlaveDevicePtr>::iterator RegisteredDevsIter;
    std::map<unsigned,I2cSlaveDevicePtr> _registeredDevs;     // devices registerd to this slave controller.
    unsigned _i2cState;
    I2cSlaveDevicePtr _currDev;        // currently addressed device

    bool     _rw;                          // substates for the state machine below
    unsigned _bitCount;                         
    uint8_t  _currByte;                 // current byte under assembly
    PinMode _sdaPinMode;
    bool     _rdValid;                  // true when the read is valid from the device...

    // state names and implementation comes from here.
    // https://www.digikey.com/eewiki/pages/viewpage.action?pageId=10125324
    //   modified as start and stop are actually events, not states

    void handleStateReady(unsigned i2cEvent, unsigned sda);
    void handleStateAddr(unsigned i2cEvent, unsigned sda);
    void handleStateAckAddr(unsigned i2cEvent, unsigned sda);
    void handleStateWr(unsigned i2cEvent, unsigned sda);
    void handleStateRd(unsigned i2cEvent, unsigned sda);
    void handleStateAck(unsigned i2cEvent, unsigned sda);
    void handleStateStopEnd(unsigned i2cEvent, unsigned sda);

private:

};


#endif