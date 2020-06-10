
#include "Particle.h"

#include "I2cSlaveCtl.h"
#include <ostream>

// photon documention for io is here:
// https://docs.particle.io/reference/device-os/firmware/photon/
// 
// specifically note: digital io and low level io, which i think we are gonig to need
//

I2cSlaveCtl::I2cSlaveCtl(int sdlGpio, int sdaGpio) :
    _sclGpio(sdlGpio),
    _sdaGpio(sdaGpio),
    _dbgPrint(nullptr),
    _i2cLastGpio(0x3),
    _i2cState(0),
    _currDev(nullptr),
    _rw(1),
    _bitCount(0),
    _currByte(0),
    _sdaPinMode(INPUT)
{
    pinMode(sdlGpio, INPUT);
    setSdaPinMode(INPUT);
}
I2cSlaveCtl::~I2cSlaveCtl() {

}

void I2cSlaveCtl::writeSda(unsigned data) 
{
    digitalWrite(_sdaGpio, data);
}
unsigned I2cSlaveCtl::readSda() 
{
    return(digitalRead(_sdaGpio));
}

void I2cSlaveCtl::setSdaPinMode(PinMode mode)
{
    if (_sdaPinMode != mode) {
        pinMode(_sdaGpio, mode);
        _sdaPinMode = mode;
    }
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
    setSdaPinMode(OUTPUT);

}
void I2cSlaveCtl::gotoStateWr(unsigned from)
{
    _i2cState = I2C_STATE_WR;
    setSdaPinMode(INPUT);

}
void I2cSlaveCtl::gotoStateRd(unsigned from)
{
    _i2cState = I2C_STATE_RD;
    setSdaPinMode(INPUT);

}
void I2cSlaveCtl::gotoStateAck(unsigned from)
{
    _i2cState = I2C_STATE_ACK;
    setSdaPinMode(OUTPUT);
}

/**
 * @brief perform an i2c sample of the signals
 *        any class calls to the i2c slave devices happen in 
 *        this step function.
 * @param -- now -- time to use to timeout a write state
 *                  that has jamed the lines too long.
 */
void I2cSlaveCtl::sampleIO(double now) {

    // think about some sort of time out if we stay in
    // a out enabled state too long...  (need to prevent a run-away write)
    //  
    // so we are in the write or ack state and we get no clocks
    //    or we get way more write bytes than any of our consumers
    //    could possibly need, don't jam the bus...
    //
    unsigned i2cGpio = (digitalRead(_sclGpio) << 1) | digitalRead(_sdaGpio);

    unsigned i2cEvent=I2C_EVENT_INVAL;
    unsigned b = ((i2cGpio ^ _i2cLastGpio)<<2) + i2cGpio;  //  bits SDL_CHANGED[3], SDA_CHANGED[2], SDL[1], SDA[0]
    if (b == 0b0110) 
        i2cEvent=I2C_EVENT_START;
    else if (b == 0b0111)
        i2cEvent=I2C_EVENT_STOP;
    else if ((b & 0b1110) == 0x1000)
        i2cEvent=I2C_EVENT_SCL_DOWN;
    else if ((b & 0b1110) == 0x1010)
        i2cEvent=I2C_EVENT_SCL_UP;

    // i2c spec says:
    //The data on the SDA line must be stable during the HIGH period of the clock. The HIGH
    // or LOW state of the data line can only change when the clock signal on the SCL line is
    // LOW (see Figure 4). One clock pulse is generated for each data bit transferred.


    switch (_i2cState) {
        case I2C_STATE_READY: {
            switch (i2cEvent) {
                case I2C_EVENT_START:
                    gotoStateAddr(_i2cState);
                    break;
                default:
                    break;          // ignore
            }
            break;
        }
        case I2C_STATE_ADDR: {
            switch (i2cEvent) {
                case I2C_EVENT_START:
                    gotoStateAddr(_i2cState);        // abort current, start again.
                    break;
                case I2C_EVENT_STOP:
                    gotoStateReady(_i2cState);               // abort, ready
                    break;
                case I2C_EVENT_SCL_UP: {
                    if ((_bitCount == 0) || (_bitCount > 8))     // this should NEVER happen, but just in case. 
                        gotoStateReady(_i2cState);          
                    _bitCount -= 1;
                    unsigned bit = readSda();
                    if (_bitCount == 0) {                       // last bit...
                        RegisteredDevsIter iter;
                        iter = _registeredDevs.find(_currByte);
                        if (iter != _registeredDevs.end()) {
                            _rw = bit;
                            _currDev = iter->second;
                            gotoStateAckAddr(_i2cState);
                        }
                        else
                            gotoStateReady(_i2cState);  // nothing registered at this address, ignore...
                        
                    }
                    else 
                        _currByte = (_currByte<<1) + bit;
                    break;
                }
                default:
                    break;              // don't care about SCL UP..
            }
            break;
        }
        case I2C_STATE_ACK_ADDR: {
            switch (i2cEvent) {
                case I2C_EVENT_START:
                    gotoStateAddr(_i2cState);        // abort current, start again.
                    break;
                case I2C_EVENT_STOP:
                    gotoStateReady(_i2cState);               // abort, ready
                    break;
                case I2C_EVENT_SCL_DOWN:
                    
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
            break;
        }
        case I2C_STATE_RD: {
            switch (i2cEvent) {
                case I2C_EVENT_START:
                    gotoStateAddr(_i2cState);        // abort current, start again.
                    break;
                case I2C_EVENT_STOP:
                    gotoStateReady(_i2cState);               // abort, ready
                    break;
                case I2C_EVENT_SCL_UP:            // sample on rising edge of scl
                    // read bits                    
                    break;
                default:
                    break;              // don't care about SCL UP..
            }
            break;
        }
        case I2C_STATE_WR: {
            switch (i2cEvent) {
                case I2C_EVENT_START:
                    gotoStateAddr(_i2cState);        // abort current, start again.
                    break;
                case I2C_EVENT_STOP:
                    gotoStateReady(_i2cState);               // abort, ready
                    break;
                case I2C_EVENT_SCL_DOWN:              // present new data on falling edge of scl
                    // write bits
                    break;
                default:
                    break;  
            }
            break;
        }
        case I2C_STATE_ACK: {
            switch (i2cEvent) {
                case I2C_EVENT_START:
                    gotoStateAddr(_i2cState);        // abort current, start again.
                    break;
                case I2C_EVENT_STOP:
                    gotoStateReady(_i2cState);               // abort, ready
                    break;
                case I2C_EVENT_SCL_DOWN:
                    
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
            break;
        }
    }
    _i2cLastGpio = i2cGpio; // parting shot, remember this.
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


