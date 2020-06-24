#ifndef __I2C_MSXXX_SIM_H__
#define __I2C_MSXXX_SIM_H__

#include "I2cSlaveDevice.h"
#include "JamventTime.h"
/**
 * @brief common functionality for MSxxx sensors... MS5803 and MS5806
 * 
 */
class I2cMSxxxSim: public I2cSlaveDevice 
{
public:
    I2cMSxxxSim() = delete;
    I2cMSxxxSim(unsigned devAddr);
    virtual ~I2cMSxxxSim() {};
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
    uint16_t crc4(uint16_t n_prom[]);
    void doConversion(unsigned conv); 
    int getPrecisionDelay(unsigned precision);
    uint32_t convTemperature(uint32_t temperature);
    uint32_t convPressure(uint32_t pressure);



    enum {
        CMD_RESET = 0x1E,           // 8 bit commands
        CMD_ADC_READ = 0x00,

        // 4 bit commands with type
        CMD_CV_D1 = 0x40,
        CMD_CV_D2 = 0x50,

        CMD_PROM_RD = 0xA0,

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
    JamventTime _jtime;         // time object...
    unsigned _lastConv;         // last conversion sent.
    double _timeConv;           // time to release the conversion.
    uint16_t _prom[8];
    uint32_t _temperature;
    uint32_t _pressure;
    uint32_t _temperature_raw;
    uint32_t _pressure_raw;


};

#endif