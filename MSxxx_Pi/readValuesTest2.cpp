/******************************************************************************
readValuesTest2.cpp
Author: AJ Ortiz
Organization: GlobalVent
Date: 2020
License: GPL V3

Most basic test to read pressure values from MS5803 and MS5603 sensor using
MSxxxx_I2C_Pi library

Second test to read pressure values from MS5803 and MS5603 sensor

Uses the individual function calls from library, which allows us to send ADC
conversion command to each sensor first, then wait once, then read
both values back..this cuts the wait time vs calling getPressure() on each
sensor.

Test notes --
  - ~19 ms per cycle at ADC_2048 for each sensor, this matches
    what we see on the Particle Photon. The ADC conversion time 2*(1+6) = 14 ms
    and then about 5 more ms for the other processing to run


g++ -o pressureTest2 readValuesTest2.cpp MSxxxx_I2C_Pi.cpp -lpigpio
sudo ./pressureTest2
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <iostream> // For debug
#include <linux/i2c-dev.h> // ???
#include <pigpio.h>


#include "MSxxxx_I2C_Pi.h"

using namespace std;

#define CONSOLEDEBUG 1
#define FS 60

void readSensorsFn(void);

// Both sensors use same address scheme, luckily MIKROE board (MS5607) is at 0x77 by default
MSxxxx p_sys_sensor(MS_ADDRESS_LOW, MS5607); //
MSxxxx p_res_sensor(MS_ADDRESS_HIGH, MS5803); // ADDRESS_HIGH is part of library

//Fixed during runtime
const double Ts = 1.0/FS;
const int Ts_micros = Ts*1e6;
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
  uint32_t startTick, stopTick; // Note wraps around after 2^32 us
while(1) {

    startTick = gpioTick();
    readSensorsFn();
    stopTick = gpioTick();
	//stepTime = gpioDelay(5000);
  stepTime = stopTick - startTick; // + stepTime; // Total elapsed time with delay, around 19000 us with no delays for a temp wait and pressure wait at ADC_2048
  cout << "delta t = " << stepTime << " us\n";
}
  gpioTerminate();
}

void readSensorsFn() {
  // Catch-all function to read both pressure sensors at once so we can wait
  // once while the ADCs on each do their conversion
    p_sys_sensor.sendADCCommand(TEMPERATURE, p_sys_resolution);
    p_res_sensor.sendADCCommand(TEMPERATURE, p_res_resolution);


    // AJO note -- faster if we predefine delay during setup based on resolutions?
    gpioDelay(1000); //general delay -- we can return the ACTUAL delay time if wanted
    switch(max_res)
    {
      case ADC_256 : gpioDelay(1000); break;
      case ADC_512 : gpioDelay(3000); break;
      case ADC_1024: gpioDelay(4000); break;
      case ADC_2048: gpioDelay(6000); break;
      case ADC_4096: gpioDelay(10000); break;
    }


    p_sys_sensor.readRawTemp();
    p_res_sensor.readRawTemp();

    // Read pressure from the sensor in mbar (temp correction applied inline)
    p_sys_sensor.sendADCCommand(PRESSURE, p_sys_resolution);
    p_res_sensor.sendADCCommand(PRESSURE, p_res_resolution);

    // AJO note -- faster if we predefine delay during setup based on resolutions?
    gpioDelay(1000); //general delay
    switch(max_res)
    {
      case ADC_256 : gpioDelay(1000); break;
      case ADC_512 : gpioDelay(3000); break;
      case ADC_1024: gpioDelay(4000); break;
      case ADC_2048: gpioDelay(6000); break;
      case ADC_4096: gpioDelay(10000); break;
    }

    p_sys_sensor.readRawPressure();
    p_res_sensor.readRawPressure();

    //Calculate actual values
    p_abs_ms5607 = p_sys_sensor.convertRawValues();
    p_abs_ms5803 = p_res_sensor.convertRawValues();

    if(CONSOLEDEBUG) {
      cout << "MS5607 pressure = " << p_abs_ms5607 << "\n";
      cout << "MS5803 pressure = " << p_abs_ms5803 << "\n";
    }
}
