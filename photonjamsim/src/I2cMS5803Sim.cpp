

#include <string.h>
#include <math.h>

#include "I2cMS5803Sim.h"

I2cMS5803Sim::I2cMS5803Sim(unsigned devAddr) : I2cMSxxxSim(devAddr){};

/**
 * @brief convTemperature
 *      convert the temperature value.
 * @param temperature_reported -- temperature to convert.
 * @return converted temperature
 */
uint32_t I2cMS5803Sim::convTemperature(uint32_t temperature_reported)
{
    // Assume temperature in deg C and pressure mbar/hPa
    // Currently only set up for actual temp > 20 C
    uint32_t rawValue;
    int32_t temp_calc = temperature_reported * 100; // - Not needed if caller set up right
    int32_t dT;

    //
    // note this probably can be done with integer arithemitic, rather than doubles
    //    but we need to deal with the shifts... in a better way...
    //

    double a = -7 / pow(2, 37);
    double b = _prom[6] / pow(2, 23);
    double c = 2000 - temp_calc;
    double soln1 = (-1 * b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
    double soln2 = (-1 * b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
    int32_t dtHighBound = 16777216;
    int32_t dtLowBound = -16776960;

    dT = 0;
    if (soln1 < dtLowBound || soln1 > dtHighBound)   {
        if (soln2 > dtLowBound || soln2 < dtHighBound)   {
            dT = soln2;
        }
    }
    else if ((soln1 > dtLowBound) && (soln1 < dtHighBound))  {
        dT = soln1;
    }
    else   {
    }


    // Raw temp
    rawValue = dT + ((int32_t)_prom[5] << 8);

    int64_t T2 = 7 * (dT * dT) / pow(2, 37);
    int64_t temp_calc_0 = temp_calc + T2;
    int64_t OFF2 = 1 * (temp_calc_0 - 2000) * (temp_calc_0 - 2000) / (pow(2, 4));

    // Update offset/sensitivty
    _OFF = ((int64_t)_prom[2] << 16) + ((((int64_t)_prom[4] * (int64_t)dT)) >> 7) - OFF2;
    _SENS = ((int64_t)_prom[1] << 15) + ((((int64_t)_prom[3] * (int64_t)dT)) >> 8);

    return rawValue;
}

/**
 * @brief convPressure 
 *      convert a pressure to a raw value
 * 
 * @param pressure -- pressure to convert
 * @return converted pressure.
 */
uint32_t I2cMS5803Sim::convPressure(uint32_t pressure_reported)
{
  uint32_t rawValue;
  int32_t pressure_calc;

  pressure_calc = pressure_reported * 10;

  rawValue = (double)( 32768 * (int64_t)pressure_calc + _OFF ) * ((double)2097152 / (double)_SENS );
  return rawValue;
}
