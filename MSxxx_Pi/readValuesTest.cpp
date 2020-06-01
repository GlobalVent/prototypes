/******************************************************************************
readValuesTest.cpp
Author: AJ Ortiz
Organization: GlobalVent
Date: 2020
License: GPL V3

Most basic test to read pressure values from MS5803 and MS5603 sensor using
MSxxxx_I2C_Pi library

Uses getPressure() function from library, which has the delay call embedded into
each call, see other example for something closer to what we'd want in practice
 (send ADC conversion command to each sensor first, then wait once, then read
  both values back..this cuts the wait time in half to get both pressure values)

g++ -o pressureTest2 readValuesTest2.cpp MSxxxx_I2C_Pi.cpp -lpigpio
sudo ./readPressureSensorValue
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <iostream> // For debug
#include <linux/i2c-dev.h> // ???
#include <pigpio.h>


#include "MSxxxx_I2C_Pi.h"

using namespace std;

#define CONSOLEDEBUG 1



// Both sensors use same address scheme, luckily MIKROE board (MS5607) is at 0x77 by default
MSxxxx p_sys_sensor(MS_ADDRESS_LOW, MS5607); //
MSxxxx p_res_sensor(MS_ADDRESS_HIGH, MS5803); // ADDRESS_HIGH is part of library

//Fixed during runtime
const double Ts = 1.0/FS;
const int Ts_millis = Ts*1e3;
const precision p_res_resolution = ADC_2048;
const precision p_sys_resolution = ADC_2048;
uint8_t max_res;

//Create variables to store results
double p_abs_ms5607, p_baseline_ms5607, p_abs_ms5803, p_baseline_ms5803;
uint16_t p_rel_analog_ms5607, p_rel_analog_ms5803;
unsigned long startTime;
unsigned int  stepTime;


int main(int argc, char *argv[])
{

  /* Setup block */
  // Compare both resolutions to see which one is higher, this will determine
  // the ADC conversion time (assuming equal sample rate for both sensors)
  if(p_res_resolution >= p_sys_resolution) {
      max_res = p_res_resolution;
  } else {
    max_res = p_sys_resolution;
  }


  if (gpioInitialise()<0) return -1;

  p_res_sensor.initI2C();
  p_res_sensor.reset();
  p_res_sensor.begin();
  gpioDelay(1000);

	p_sys_sensor.initI2C();
  p_sys_sensor.reset();
  p_sys_sensor.begin();


  // Read pressure from the sensors in mbar (temp correction applied inline)
  // AJO TO-DO - Do multiple readings here and average?
  p_baseline_ms5607 = p_sys_sensor.getPressure(ADC_4096);
  p_baseline_ms5803 = p_res_sensor.getPressure(ADC_4096);

  if(CONSOLEDEBUG) {
    cout << "MS5607 baseline pressure = " << p_baseline_ms5607 << " mbar\n";
    cout << "MS5803 baseline pressure = " << p_baseline_ms5803 << " mbar\n";
  }

  /* Loop block */

while(1) {
    p_abs_ms5607 = p_sys_sensor.getPressure(ADC_2048);
    p_abs_ms5803 = p_res_sensor.getPressure(ADC_2048);
    cout << "MS5607 pressure = " << p_abs_ms5607 << "\n";
    cout << "MS5803 pressure = " << p_abs_ms5803 << "\n";
	gpioDelay(1000);
}

  gpioTerminate();

}
