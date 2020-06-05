
#include "Particle.h"
#include "I2cSlaveCtl.h"


I2cSlaveCtl::I2cSlaveCtl(int sdlGpio, int sdaGpio) :
    _sdlGpio(sdlGpio),
    _sdaGpio(sdaGpio),
    _i2cLastSample(0x3),
    _i2cState(0),
    _read(false) 
{
    pinMode(sdlGpio, INPUT);
    pinMode(sdaGpio, INPUT);


}
I2cSlaveCtl::~I2cSlaveCtl() {

}


/**
 * @brief perform an i2c sample of the signals
 *        any class calls to the i2c slave devices happen in 
 *        this step function.
 * 
 */
void I2cSlaveCtl::sampleIO() {

}

/**
 * @brief register the i2c device with the controller.
 * 
 * @param dev pointer to the device to regsiter.
 * @return true if successful fasle id already in use.
 */
bool I2cSlaveCtl::registerI2cDevice(I2cSlaveDevice *dev) {


    return(true);
}

