
#ifndef __I2C_MS5803SIM_H__
#define __I2C_MS5803SIM_H__

#include "I2cMSxxxSim.h"

/**
 * @brief I2cMS5603Sim -- photonjamsim slave config i2c device
 * 
 */
class I2cMS5803Sim : public I2cMSxxxSim
{
public:
    I2cMS5803Sim() = delete;
    /**
     * @brief Construct a new  object
     * 
     * @param devAddr -- device address for this device.
     */
    I2cMS5803Sim(unsigned devAddr);

protected:
    virtual uint32_t convTemperature(uint32_t temperature);
    virtual uint32_t convPressure(uint32_t pressure);


private:
};
#endif
