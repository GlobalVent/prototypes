
#include <Particle.h>
#include "I2cIntHandler.h"


#define DBG_OUT A2
// singleton instance only..
I2cIntHandler *I2cIntHandler::_staticThis = nullptr;

I2cIntHandler::I2cIntHandler() :
    _dbgPrint(nullptr) 
{
    pinMode(DBG_OUT,OUTPUT);
    _staticThis = this;
}

void I2cIntHandler::staticIntHandler() 
{
    if (_staticThis == nullptr)
        return;
    _staticThis->handleInterrupt();
}


/**
 * @brief get the total time in ms since we started.
 * 
 * @return total number of microseconds since we started.
 *          NOTE: this handles the roll over if called more than once an hour.
 */
uint64_t I2cIntHandler::microsNow() 
{        
    static uint32_t lastNow_us=0;  // check for overflow.
    static uint64_t totalNow_us=0;
    uint64_t delta_us;
    uint32_t now_us = micros();
    if (now_us >= lastNow_us) 
        delta_us = now_us-lastNow_us;
    else                // we rolled over..
        delta_us = (0x100000000ull - lastNow_us) + now_us;
    lastNow_us = now_us;
    totalNow_us += delta_us;
    return(totalNow_us);
}

void I2cIntHandler::detachAllInterrupts() {
    for (auto ctl : _registeredCtls) 
        detachInterrupt(ctl.second->getSdaGpio());
}
void I2cIntHandler::attachAllInterrupts() {
    for (auto ctl : _registeredCtls) 
        attachInterrupt(ctl.second->getSdaGpio(), staticIntHandler, FALLING, 0);
}

void I2cIntHandler::handleInterrupt() 
{
    bool detectedStart = false;
    //digitalWriteFast(DBG_OUT,1);
    for (auto ctl : _registeredCtls) {
        unsigned i2cEvent=ctl.second->decodeEvent(pinReadFast(ctl.second->getSclGpio()),
                                                  pinReadFast(ctl.second->getSdaGpio()));
        if (i2cEvent == ctl.second->I2C_EVENT_START) {
            detectedStart = true;
            break;              // we detected one, so now we are going into poll mode...
        }
    }
    //digitalWriteFast(DBG_OUT, 0); 
    if (detectedStart) {
        if (_dbgPrint) _dbgPrint->printf("detected start\n");
        uint64_t start = microsNow();       
        uint64_t end = start + 600*10;   // just something see with a scope...
        detachAllInterrupts();
        //digitalWriteFast(DBG_OUT, 1); 
                                       // 
        while (microsNow() < end) {  
            for (auto ctl: _registeredCtls) {
                unsigned i2cEvent=ctl.second->decodeEvent(pinReadFast(ctl.second->getSclGpio()),
                                                          pinReadFast(ctl.second->getSdaGpio()));
                if (i2cEvent) {
                    // todo extend the timeout...
                }                           
           }
        }
        for (auto ctl: _registeredCtls) 
            ctl.second->resetI2cState();
        attachAllInterrupts();
        //digitalWriteFast(DBG_OUT,0);
        // make sure pin mode for SDA is set...
        // then attach all interrupts.
        if (_dbgPrint) _dbgPrint->printf("return isr\n");

    }
}

bool I2cIntHandler::registerI2cSlaveCtl(I2cSlaveCtl *ctl)
{
    unsigned sclsda = (ctl->getSclGpio() << 8) + ctl->getSdaGpio();
    if (_registeredCtls.find(sclsda) != _registeredCtls.end()) {
        return(false);
    }
    _registeredCtls[sclsda] = ctl;
    return(true);
}

