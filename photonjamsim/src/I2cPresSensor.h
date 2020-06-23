
#ifndef __I2C_PRES_SENSOR_H__
#define __I2C_PRES_SENSOR_H__

#include "I2cSlaveDevice.h"

/**
 * @brief I2cPresSensor -- photonjamsim slave config i2c device
 * 
 */
class I2cPresSensor : public I2cSlaveDevice
{
public:
    I2cPresSensor() = delete;
    /**
     * @brief Construct a new I2cSlaveDevice object
     * 
     * @param devAddr -- device address for this device.
     */
    I2cPresSensor(unsigned devAddr);

    /**
     * @brief stop event received AFTER receiving a start event...
     * 
     * @param -- _rw -- read/write_
     */
    virtual void stop(unsigned _rw);


    
    /**
     * @brief Set the Pressure
     * 
     * @param pressure 
     */
    virtual void setPressure(unsigned pressure) {
        _pressure = pressure;
    }
    /**
     * @brief Set the Temperature
     * 
     * @param temperature 
     */
    virtual void setTemperature(unsigned temperature) {
        _temperature = temperature;
    }


protected:
    void clearSendData();

    enum {
        CMD_RESET = 0x1E,           // 8 bit commands
        CMD_ADC_READ = 0x00,

        // 4 bit commands with type
        CMD_CV_D1 = 0x4,
        CMD_CV_D2 = 0x5,

        CMD_PROM_RD = 0xA,

        CMD_OSR_256  = 0x00,        // osr type (precisions)
        CMD_OSR_512  = 0x02,
        CMD_OSR_1024 = 0x04,
        CMD_OSR_2048 = 0x06,
        CMD_OSR_4096 = 0x08,
        CMD_OSR_8192 = 0x0A,

        // extended commands        // for testing.
        CMD_SET_TEMP = 0x10,
        CMD_SET_PRES = 0x11,
    };
    uint16_t _prom[8];
    uint32_t _temperature;
    uint32_t _pressure;

private:
};
#endif
