/******************************************************************************
MSxxxx_I2C.cpp
Author: AJ Ortiz
Organization: GlobalVent
Date: 2020
License: GPL V3

Generic Raspberry Pi library for using a variety of MSxxxx pressure sensors.
Currently supports - MS5803, MS5607

NOTE - for pigpio we do the gpioInitialize and gpioTerminate from the caller
function, not from within this library!

Original reference ---
Library for MS5607 pressure sensors.
Casey Kuhns @ SparkFun Electronics
6/26/2014
https://github.com/sparkfun/MS5607-14BA_Breakout
******************************************************************************/

#include <pigpio.h> // Library used for I2C on Pi
#include <math.h> //Can take this out if we just bring pow() function in-line
#include "MSxxxx_I2C_Pi.h"

//#define DEBUGMODE

#ifdef DEBUGMODE
#include <iostream>
using namespace std;
#endif

MSxxxx::MSxxxx(ms_addr address, ms_model ms_model)
// Base library type I2C
{
  _address = address; //set interface used for communication
  _ms_model = ms_model;
}

MSxxxx::~MSxxxx()
// Close out device I2C
{
  i2cClose(_handle);
}

void MSxxxx::initI2C() {
  // Separate from constructor because we have to do a gpioInitialise() first
  _handle = i2cOpen(1, _address, 0); // NOTE: Assumes Pi with i2cbus 1 (all after model B)
  #ifdef DEBUGMODE
  cout << "i2c handle -- " << _handle << "\n";
  #endif
}

void MSxxxx::reset(void)
// Reset device I2C
{
  i2cWriteByte(_handle,CMD_RESET);
  sensorWait(5000); //5 ms wait - extended this for r pi to 5ms
}

int8_t MSxxxx::begin(void)
{
  // Initialize library for subsequent pressure measurements
  // We use 16-bit coefficients
  uint8_t i;
  int tmp;
  char buf[2];
  int status;


  for(i = 0; i <= 7; i++){
    // Can use return values here to check for errors
    i2cWriteByte(_handle, CMD_PROM + (i*2));
    //sensorWait(1000);
    status = i2cReadDevice(_handle, buf, 2);
    coefficient[i] = (buf[0] << 8)| buf[1];

    if(status == PI_I2C_READ_FAILED) {
      #ifdef DEBUGMODE
      cout << "I2C read failed for coefficient using i2cReadDevice\n";
      #endif
      return PI_I2C_READ_FAILED;
    }

    #ifdef DEBUGMODE
    cout << "i2cReadDevice -- coefficient[" << int(i) << "] = " << coefficient[i] << endl;
    #endif
  }

  return 0;
}

float MSxxxx::getTemperature(temperature_units units, precision _precision)
// Return a temperature reading in either F or C.
{

  _temperature_raw = getADCconversion(TEMPERATURE, _precision);
  _pressure_raw = getADCconversion(PRESSURE, _precision);
  convertRawValues();
  float temperature_reported;
  // If Fahrenheit is selected return the temperature converted to F
  if(units == FAHRENHEIT){
    temperature_reported = _temperature_actual / 100;
    temperature_reported = (((temperature_reported) * 9) / 5) + 32;
    return temperature_reported;
  }

  // If Celsius is selected return the temperature converted to C
  else {
    temperature_reported = _temperature_actual / 100;
    return temperature_reported;
  }
}

float MSxxxx::getPressure(precision _precision)
// Return a pressure reading units Pa.
{
  //Retrieve ADC result
  _temperature_raw = getADCconversion(TEMPERATURE, _precision);
  _pressure_raw = getADCconversion(PRESSURE, _precision);
  convertRawValues();
  float pressure_reported;
  pressure_reported = _pressure_actual;

  if(_ms_model == MS5803) {
    pressure_reported = pressure_reported / 10;
  } else if(_ms_model == MS5607) {
    pressure_reported = pressure_reported / 100;
  }
  return pressure_reported;
}

// Apply compensation to get actual pressure/temp values (keeping "conversion" out
// of function name to avoid confusion with ADC conversion)
// Returns final pressure value
float MSxxxx::convertRawValues() {
  float pressure_reported;

  #ifdef DEBUGMODE
    uint32_t startTick, stopTick, totalTime;
    startTick = gpioTick();
  #endif

  if(_ms_model == MS5803) {
    //Create Variables for calculations
    int32_t temp_calc;
    int32_t pressure_calc;

    int32_t dT;

    //Now that we have a raw temperature, let's compute our actual.
    dT = _temperature_raw - ((int32_t)coefficient[5] << 8);
    temp_calc = (((int64_t)dT * coefficient[6]) >> 23) + 2000;

    // TODO TESTING  _temperature_actual = temp_calc;

    //Now we have our first order Temperature, let's calculate the second order.
    int64_t T2, OFF2, SENS2, OFF, SENS; //working variables

    if (temp_calc < 2000)
    // If temp_calc is below 20.0C
    {
      T2 = 3 * (((int64_t)dT * dT) >> 33);
      OFF2 = 3 * ((temp_calc - 2000) * (temp_calc - 2000)) / 2;
      SENS2 = 5 * ((temp_calc - 2000) * (temp_calc - 2000)) / 8;

      if(temp_calc < -1500)
      // If temp_calc is below -15.0C
      {
        OFF2 = OFF2 + 7 * ((temp_calc + 1500) * (temp_calc + 1500));
        SENS2 = SENS2 + 4 * ((temp_calc + 1500) * (temp_calc + 1500));
      }
    }
    else
    // If temp_calc is above 20.0C
    {
      T2 = 7 * ((uint64_t)dT * dT)/pow(2,37);
      OFF2 = ((temp_calc - 2000) * (temp_calc - 2000)) / 16;
      SENS2 = 0;
    }

    // Now bring it all together to apply offsets

    OFF = ((int64_t)coefficient[2] << 16) + (((coefficient[4] * (int64_t)dT)) >> 7);
    SENS = ((int64_t)coefficient[1] << 15) + (((coefficient[3] * (int64_t)dT)) >> 8);

    temp_calc = temp_calc - T2;
    OFF = OFF - OFF2;
    SENS = SENS - SENS2;

    // Now lets calculate the pressure
    pressure_calc = (((SENS * _pressure_raw) / 2097152 ) - OFF) / 32768;

    _temperature_actual = temp_calc ;
    _pressure_actual = pressure_calc ; // 10;// pressure_calc;

    pressure_reported = _pressure_actual;
    pressure_reported = pressure_reported / 10;
  }
  else if(_ms_model == MS5607)
  {
    //Create Variables for calculations
    int32_t temp_calc;
    int32_t pressure_calc;

    int32_t dT;

    //Now that we have a raw temperature, let's compute our actual.
    dT = _temperature_raw - ((int32_t)coefficient[5] << 8);
    temp_calc = (((int64_t)dT * coefficient[6]) >> 23) + 2000;

    // TODO TESTING  _temperature_actual = temp_calc;

    //Now we have our first order Temperature, let's calculate the second order.
    int64_t T2, OFF2, SENS2, OFF, SENS; //working variables

    if (temp_calc < 2000)
    // If temp_calc is below 20.0C
    {
      T2 = ((int64_t)dT * dT) >> 31;
      OFF2 = 61 * ((temp_calc - 2000) * (temp_calc - 2000)) / 16;
      SENS2 = 2 * ((temp_calc - 2000) * (temp_calc - 2000));

      if(temp_calc < -1500)
      // If temp_calc is below -15.0C
      {
        OFF2 = OFF2 + 15 * ((temp_calc + 1500) * (temp_calc + 1500));
        SENS2 = SENS2 + 8 * ((temp_calc + 1500) * (temp_calc + 1500));
      }
    }
    else
    // If temp_calc is above 20.0C
    {
      T2 = 0;
      OFF2 = 0;
      SENS2 = 0;
    }

    // Now bring it all together to apply offsets

    OFF = ((int64_t)coefficient[2] << 17) + (((coefficient[4] * (int64_t)dT)) >> 6);
    SENS = ((int64_t)coefficient[1] << 16) + (((coefficient[3] * (int64_t)dT)) >> 7);

    temp_calc = temp_calc - T2;
    OFF = OFF - OFF2;
    SENS = SENS - SENS2;

    // Now lets calculate the pressure


    pressure_calc = (((SENS * _pressure_raw) / 2097152 ) - OFF) / 32768;

    _temperature_actual = temp_calc ;
    _pressure_actual = pressure_calc ; // 10;// pressure_calc;

    pressure_reported = _pressure_actual;
    pressure_reported = pressure_reported / 100;
  }

  #ifdef DEBUGMODE
    stopTick = gpioTick();
    totalTime = stopTick - startTick;
    cout << "Total time for temp/pressure calculation: " << totalTime << " us\n";
  #endif
  return pressure_reported;
}

void MSxxxx::sendADCCommand(measurement _measurement, precision _precision) {
  i2cWriteByte(_handle, CMD_ADC_CONV + _measurement + _precision);
}

void MSxxxx::readRawTemp() {
  // Reads temperature value from the bus and sets variable
  _temperature_raw = readValue();
}

void MSxxxx::readRawPressure() {
  // Read the raw pressure value from the bus and sets variable
  _pressure_raw = readValue();
}

// Note - when calling after sendADCCommand, wait to read from ADC until
// appropriate ADC conversion time has passed (see datasheet, resolution-dependent)
uint32_t MSxxxx::readValue() {
  uint32_t result;
  uint8_t highByte, midByte, lowByte;
  int status;
  char buf[3];

  i2cWriteByte(_handle, CMD_ADC_READ);
  // //sensorWait(1000);
  status = i2cReadDevice(_handle, buf, 3); // Read 3 bytes

  highByte = buf[0];
  midByte = buf[1];
  lowByte = buf[2];

  result = ((uint32_t)highByte << 16) + ((uint32_t)midByte << 8) + lowByte;

  return result;
}

uint32_t MSxxxx::getADCconversion(measurement _measurement, precision _precision)
// Retrieve ADC measurement from the device.
// Select measurement type and precision
{
  int64_t result;
  int16_t highByte, midByte, lowByte;
  int status;
  char buf[3];

  status = i2cWriteByte(_handle, CMD_ADC_CONV + _measurement + _precision);

  #ifdef DEBUGMODE
  cout << "Write CMD_ADC_CONV returned: " << status << "\n";
  #endif

  // Wait for conversion to complete
  sensorWait(1000); //general delay
  switch( _precision )
  {
    case ADC_256 : sensorWait(1000); break;
    case ADC_512 : sensorWait(3000); break;
    case ADC_1024: sensorWait(4000); break;
    case ADC_2048: sensorWait(6000); break;
    case ADC_4096: sensorWait(10000); break;
  }

  status = i2cWriteByte(_handle, CMD_ADC_READ);
  //sensorWait(1000);
  status = i2cReadDevice(_handle, buf, 3); // Read 3 bytes


  highByte = buf[0];
  midByte = buf[1];
  lowByte = buf[2];

  result = ((uint32_t)highByte << 16) + ((uint32_t)midByte << 8) + lowByte;

  /*if(status == PI_I2C_READ_FAILED || status != 3) {
  result = PI_I2C_READ_FAILED; // NOTE - result is a uint32 but the failure code is signed (-83)
} */

//result = PI_I2C_WRITE_FAILED;

#ifdef DEBUGMODE
  if(status == PI_I2C_READ_FAILED) {
      cout << "I2C read failed for coefficient using i2cReadI2CBlockData\n";
    }

    cout << "Write CMD_ADC_READ returned: ";
    cout << status << endl;
    cout << "reading returned " << result << "\n";
#endif

return result;

}

void MSxxxx::sendCommand(uint8_t command)
{
  i2cWriteByte(_handle, command );
}

void MSxxxx::sensorWait(uint32_t time_us)
// Delay function. Uses pigpio microsecond delay function
{
  gpioDelay(time_us);
};
