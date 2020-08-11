

#include <math.h>

#include "I2cMS5607Sim.h"

I2cMS5607Sim::I2cMS5607Sim(unsigned devAddr) : I2cMSxxxSim(devAddr){};

/**
 * @brief convTemperature
 *      convert the temperature value.
 * @param temperature -- temperature to convert.
 * @return converted temperature
 */
uint32_t I2cMS5607Sim::convTemperature(uint32_t temperature_reported)
{
    // Assume temperature in deg C and pressure mbar/hPa
    // Currently only set up for actual temp > 20 C
    uint32_t rawValue;
    int32_t temp_calc = temperature_reported * 100; // - Not needed if caller set up right

    int32_t dT = ((int64_t)temp_calc - 2000) / ((float)_prom[6] / pow(2, 23)); // Type cast to larger?

    // Raw temp
    rawValue = dT + ((int32_t)_prom[5] << 8);

    // Update offset/sensitivty
    _OFF = ((int64_t)_prom[2] << 17) + ((((int64_t)_prom[4] * (int64_t)dT)) >> 6);
    _SENS = ((int64_t)_prom[1] << 16) + ((((int64_t)_prom[3] * (int64_t)dT)) >> 7);
    if (_SENS == 0)
        _SENS = 1;

    return rawValue;
}

/**
 * @brief convPressure 
 *      convert a pressure to a raw value
 * 
 * @param pressure -- pressure to convert
 * @return converted pressure.
 */
uint32_t I2cMS5607Sim::convPressure(uint32_t pressure_reported)
{
    uint32_t rawValue;
    int32_t pressure_calc;

    pressure_calc = pressure_reported * 100;

    rawValue = (double)(32768 * (int64_t)pressure_calc + _OFF) * ((double)2097152 / (double)_SENS);

    return rawValue;
}
