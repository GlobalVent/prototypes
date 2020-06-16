#ifndef __I2C_INT_HANDLER_H__
#define __I2C_INT_HANDLER_H__


#include <vector>
#include "I2cSlaveCtl.h"

class I2cIntHandler
{
public:
    I2cIntHandler();
    virtual ~I2cIntHandler() {};

    /**
     * @brief handleInterrupt
     *        main interrupt handler function, called on SDA change..
     */
    void handleInterrupt();

    /**
     * @brief register an i2c slave controller.
     * 
     * @param ctl -- pointer to slave controller to register
     * @return true -- device registred
     * @return false -- duplicate gpio on another device already registered.
     */
    bool registerI2cSlaveCtl(I2cSlaveCtl *ctl);

    /**
     * @brief get the total time in ms since we started.
     * 
     * @return total number of microseconds since we started.
     *          NOTE: this handles the roll over if called more than once an hour.
     */
    uint64_t microsNow();

    /**
     * @brief Set the Dbg Print pointer
     * 
     * @param dbgPrint -- pointer to dbg print object for debug telemetry
     */
    void setDbgPrint(JamsimDbgPrint *dbgPrint) {
        _dbgPrint = dbgPrint;
    }

    /**
     * @brief attach us to all interrupts...  call once during "setup"
     * 
     */
    void attachAllInterrupts();

    /**
     * @brief detach us from all interrupts...
     * 
     */
    void detachAllInterrupts();

    static void staticIntHandler();
    static I2cIntHandler *_staticThis;
protected:
    std::map<unsigned,I2cSlaveCtl *> _registeredCtls;     // devices registerd to this slave controller.
    JamsimDbgPrint *_dbgPrint;

private:

};


#endif
