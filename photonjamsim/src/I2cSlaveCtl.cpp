
#include "Particle.h"

#include "I2cSlaveCtl.h"
#include <ostream>

#define DBG_OUT A2

// photon documention for io is here:
// https://docs.particle.io/reference/device-os/firmware/photon/
// 
// specifically note: digital io and low level io, which i think we are gonig to need
//

I2cSlaveCtl::I2cSlaveCtl(int sdlGpio, int sdaGpio) :
    _sclGpio(sdlGpio),
    _sdaGpio(sdaGpio),
    _i2cLastGpio(0x3),
    _dbgPrint(nullptr),
    _i2cState(0),
    _currDev(nullptr),
    _rw(1),
    _bitCount(0),
    _currByte(0),
    _sdaPinMode(INPUT)
{
    pinMode(sdlGpio, INPUT);
    pinMode(sdaGpio, INPUT);
}
I2cSlaveCtl::~I2cSlaveCtl() {

}


void I2cSlaveCtl::setSdaPinMode(PinMode mode)
{
    if (_sdaPinMode != mode) {
        pinMode(_sdaGpio, mode);
        _sdaPinMode = mode;
    }
}
/**
 * @brief reset the i2cState -- reset to the ready state.
 * 
 */
void I2cSlaveCtl::resetI2cState() { 
    _i2cLastGpio = 0x3;
    gotoStateReady(_i2cState);
}


void I2cSlaveCtl::gotoStateReady(unsigned from) 
{
    _i2cState = I2C_STATE_READY;
    setSdaPinMode(INPUT);
    _currDev = nullptr;
    _currByte = 0;
    _bitCount = 0;
}
void I2cSlaveCtl::gotoStateAddr(unsigned from)
{
    _i2cState = I2C_STATE_ADDR;
    _bitCount = 8;         // count down 8 bits
    _currDev = nullptr;
    _currByte = 0;
    setSdaPinMode(INPUT);

}
void I2cSlaveCtl::gotoStateAckAddr(unsigned from)
{
    _i2cState = I2C_STATE_ACK_ADDR;
    _currByte = 0;
    setSdaPinMode(INPUT);       // make sure we are listening, we go to turn the line around when sdl drops.
    if (_currDev) {
        if (_rw) 
            _currByte = _currDev->read();   // we output bits until it tells us we have no more...
        else
            _currDev->write(_currByte);
    }
}
void I2cSlaveCtl::gotoStateWr(unsigned from)
{
    _i2cState = I2C_STATE_WR;
    _bitCount = 8;         // count down 8 bits
}
void I2cSlaveCtl::gotoStateRd(unsigned from)
{
    _i2cState = I2C_STATE_RD;
    _bitCount = 8;         // count down 8 bits
}
void I2cSlaveCtl::gotoStateAck(unsigned from)
{
    _i2cState = I2C_STATE_ACK;
    _currByte = 0;
    if (_currDev) {
        if (_rw) 
            _currByte = _currDev->read();   // we output bits until it tells us we have no more...
        else
            _currDev->write(_currByte);
    }
}
void I2cSlaveCtl::gotoStateStopEnd(unsigned from) 
{
    _i2cState = I2C_STATE_STOP_END;     // wait for the final SCL_UP or another start..
}

void I2cSlaveCtl::gotoStateRdEnd(unsigned from) 
{
    _i2cState = I2C_STATE_RD_END;
}



void I2cSlaveCtl::handleStateReady(unsigned i2cEvent, unsigned sda)
{
    switch (i2cEvent) {
        case I2C_EVENT_START:
            gotoStateAddr(_i2cState);
            break;
        default:
            break;          // ignore
    }
}
void I2cSlaveCtl::handleStateAddr(unsigned i2cEvent, unsigned sda)
{
    switch (i2cEvent) {
        case I2C_EVENT_START:
            gotoStateAddr(_i2cState);        // abort current, start again.
            break;
        case I2C_EVENT_STOP:
            gotoStateReady(_i2cState);               // abort, ready
            break;
        case I2C_EVENT_SCL_UP: 
            if ((_bitCount == 0) || (_bitCount > 8))     // this should NEVER happen, but just in case. 
                gotoStateReady(_i2cState);          
            _bitCount -= 1;
            if (_bitCount == 0) {                       // last bit...
                RegisteredDevsIter iter = _registeredDevs.find(_currByte);
                if (iter != _registeredDevs.end()) {
                    _rw = sda;
                    _currDev = iter->second;
                    gotoStateAckAddr(_i2cState);
                } else {
                    gotoStateReady(_i2cState);  // nothing registered at this address, ignore...
                }
                
            } else {
                _currByte = (_currByte<<1) + sda;
            }
            break;
        
        default:
            break;              // don't care about SCL UP..
    }

}
void I2cSlaveCtl::handleStateAckAddr(unsigned i2cEvent, unsigned sda)
{
    switch (i2cEvent) {
        case I2C_EVENT_START:
            gotoStateAddr(_i2cState);        // abort current, start again.
            break;
        case I2C_EVENT_STOP:
            gotoStateReady(_i2cState);               // abort, ready
            break;
        case I2C_EVENT_SCL_DOWN:
            setSdaPinMode(OUTPUT);
            digitalWrite(_sclGpio, LOW);
            break;
        case I2C_EVENT_SCL_UP:
            if (_rw)
                gotoStateRd(_i2cState);
            else
                gotoStateWr(_i2cState);
            break;
        default:
            break;              
    }

}
void I2cSlaveCtl::handleStateWr(unsigned i2cEvent, unsigned sda)
{
    switch (i2cEvent) {
        case I2C_EVENT_START:
            gotoStateAddr(_i2cState);        // abort current, start again.
            break;
        case I2C_EVENT_STOP:
            gotoStateReady(_i2cState);               // abort, ready
            break;
        case I2C_EVENT_SCL_UP:                  // sample bits on the rising edge of SCL
            if ((_bitCount == 0) || (_bitCount > 8))     // this should NEVER happen, but just in case. 
                gotoStateReady(_i2cState);          
            _bitCount -= 1;
            _currByte = (_currByte<<1) + sda;
            if (_bitCount == 0)                        // last bit...
                gotoStateAck(_i2cState);
            break;
        case I2C_EVENT_SCL_DOWN:
            setSdaPinMode(INPUT);       // transition to input mode (this is idempotent)
            break;

        default:
            break;  
    }

}
void I2cSlaveCtl::handleStateRd(unsigned i2cEvent, unsigned sda)
{
    switch (i2cEvent) {
        case I2C_EVENT_START:
            gotoStateAddr(_i2cState);        // abort current, start again.
            break;
        case I2C_EVENT_STOP:
            gotoStateReady(_i2cState);               // abort, ready
            break;
        case I2C_EVENT_SCL_DOWN: {                   // change data on falling edge of scl
            if ((_bitCount == 0) || (_bitCount > 8))     // this should NEVER happen, but just in case. 
                gotoStateReady(_i2cState);          
            _bitCount -= 1;
            unsigned bit = !!(_currByte & 0x80);
            _currByte <<= 1;
            if (bit) {
                setSdaPinMode(INPUT);       // let it float
            } else {
                setSdaPinMode(OUTPUT);
                digitalWriteFast(_sdaGpio, 0);
            }
            break;
        }
        case I2C_EVENT_SCL_UP: 
            if (_bitCount == 0)                       // last bit...
                gotoStateAck(_i2cState);
            break;
        default:
            break;              // don't care about SCL UP..
    }

}
void I2cSlaveCtl::handleStateAck(unsigned i2cEvent, unsigned sda)
{
    switch (i2cEvent) {
        case I2C_EVENT_START:
            gotoStateAddr(_i2cState);        // abort current, start again.
            break;
        case I2C_EVENT_STOP:
            gotoStateStopEnd(_i2cState);               // abort, ready
            break;
        case I2C_EVENT_SCL_DOWN:
            if (!_rw) {
                setSdaPinMode(OUTPUT);              // transition can happen only when SCL is low...
                digitalWrite(_sclGpio, LOW);
            }
            else  {
                setSdaPinMode(INPUT);   // let the master tell us on the SCL up if we are done
            }
            
            break;
        case I2C_EVENT_SCL_UP:
            if (_rw) {
                if (sda)                // sda high, NAK from host, it is done reading.
                    gotoStateRdEnd(_i2cState);
                else
                    gotoStateRd(_i2cState); // sda low, ther there is  more to read
            }
            else {
                gotoStateWr(_i2cState);
            }
            break;
        default:
            break;              
    }

}

void I2cSlaveCtl::handleStateStopEnd(unsigned i2cEvent, unsigned sda) 
{
    switch (i2cEvent) {
        case I2C_EVENT_START:
            gotoStateAddr(_i2cState);
            break;
        case I2C_EVENT_SCL_UP:
            gotoStateReady(_i2cState);
            break;
        case I2C_EVENT_SCL_DOWN:
            setSdaPinMode(INPUT);               // make sure we are in input mode.
            break;
        default:
            break;              
    }

}

void I2cSlaveCtl::handleStateRdEnd(unsigned i2cEvent, unsigned sda)
{
    switch (i2cEvent) {
        case I2C_EVENT_START:
            gotoStateAddr(_i2cState);
            break;
        case I2C_EVENT_STOP:
            gotoStateReady(_i2cState);
            break;
        case I2C_EVENT_SCL_DOWN:
            setSdaPinMode(INPUT);               // make sure we are in input mode.
            break;
        default:
            break;              
    }

}

/**
 * @brief register the i2c device with the controller.
 * 
 * @param dev pointer to the device to regsiter.
 * @return true if successful fasle id already in use.
 */
bool I2cSlaveCtl::registerI2cDevice(I2cSlaveDevice *dev) 
{
    if (_registeredDevs.find(dev->getDevAddr()) != _registeredDevs.end()) {
        _lastError = "duplicate device address";
        return(false);
    }
    _registeredDevs[dev->getDevAddr()] = dev;
    return(true);
}

/**
 * @brief decode an i2c event from the current scl,sda AND the lastGpio seen by this 
 *        controller.
 * 
 * @param lastGpio 
 * @param scl 
 * @param sda 
 * @return i2c event... 
 */
unsigned I2cSlaveCtl::decodeEvent(unsigned scl, unsigned sda)
{
    unsigned i2cGpio = (scl << 1) | sda;
    unsigned b = ((i2cGpio ^ _i2cLastGpio)<<2) + i2cGpio;  //  bits SDL_CHANGED[3], SDA_CHANGED[2], SDL[1], SDA[0]
    if (_i2cLastGpio == i2cGpio)
        return(I2C_EVENT_NC);

    //if (_dbgPrint) _dbgPrint->printf("b=0x%x\n", b);
    unsigned i2cEvent=I2C_EVENT_INVAL;
    _i2cLastGpio = i2cGpio;
    if ((b == 0b0110) || (b == 0b1110))   
        i2cEvent=I2C_EVENT_START;
    else if (b == 0b0111)
        i2cEvent=I2C_EVENT_STOP;
    else if ((b & 0b1110) == 0b1000)
        i2cEvent=I2C_EVENT_SCL_DOWN;
    else if ((b & 0b1110) == 0b1010)
        i2cEvent=I2C_EVENT_SCL_UP;
    // i2c spec says:
    // The data on the SDA line must be stable during the HIGH period of the clock. The HIGH
    //   or LOW state of the data line can only change when the clock signal on the SCL line is
    //   LOW (see Figure 4). One clock pulse is generated for each data bit transferred.

    if (i2cEvent != I2C_EVENT_INVAL) {
        digitalWriteFast(DBG_OUT, 1); 

        switch (_i2cState) {
            case I2C_STATE_READY:    handleStateReady(i2cEvent,  sda); break;
            case I2C_STATE_ADDR:     handleStateAddr(i2cEvent,   sda); break;
            case I2C_STATE_ACK_ADDR: handleStateAckAddr(i2cEvent,sda); break;
            case I2C_STATE_RD:       handleStateRd(i2cEvent,     sda); break;
            case I2C_STATE_WR:       handleStateWr(i2cEvent,     sda); break;
            case I2C_STATE_ACK:      handleStateAck(i2cEvent,    sda); break;
            case I2C_STATE_STOP_END: handleStateStopEnd(i2cEvent,sda); break;
            case I2C_STATE_RD_END:   handleStateRdEnd(i2cEvent,  sda); break;
        }
        if (_dbgPrint) _dbgPrint->printf("ev b=0x%x,e=%d,s=%d,d=%d\n", b,i2cEvent, _i2cState, sda);
        digitalWriteFast(DBG_OUT, 0); 
    }
    return(i2cEvent);
}



