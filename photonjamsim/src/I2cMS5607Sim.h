#ifndef __I2C_MS5607_SIM_H__
#define __I2C_MS5607_SIM_H__

#include "I2cMSxxxSim.h"

/**
 * @brief I2cMS5607Sim -- photonjamsim slave config i2c device
 * 
 */
class I2cMS5607Sim : public I2cMSxxxSim
{
public:
    I2cMS5607Sim() = delete;
    /**
     * @brief Construct a new I2cSlaveDevice object
     * 
     * @param devAddr -- device address for this device.
     */
    I2cMS5607Sim(unsigned devAddr);

protected:
private:
};
#endif
