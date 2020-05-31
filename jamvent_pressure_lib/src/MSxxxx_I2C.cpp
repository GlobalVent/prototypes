/******************************************************************************
MSxxxx_I2C.cpp
Author: AJ Ortiz
Organization: GlobalVent
Date: 2020
License: GPL V3

Generic library for using a variety of MSxxxx pressure sensors.
Currently supports - MS5803, MS5607

Uses Wire() library


Original reference ---
MS5607_I2C.h
Library for MS5607 pressure sensors.
Casey Kuhns @ SparkFun Electronics
6/26/2014
https://github.com/sparkfun/MS5607-14BA_Breakout
******************************************************************************/

#include <Wire.h> // Wire library is used for I2C
#include "MSxxxx_I2C.h"

MSxxxx::MSxxxx(ms_addr address, ms_model ms_model)
// Base library type I2C
{
  Wire.begin(); // Arduino Wire library initializer
  _address = address; //set interface used for communication
  _ms_model = ms_model;
}

void MSxxxx::reset(void)
// Reset device I2C
{
  sendCommand(CMD_RESET);
  sensorWait(3);
}

uint8_t MSxxxx::begin(void)
// Initialize library for subsequent pressure measurements
{
  uint8_t i;
  for(i = 0; i <= 7; i++){
    sendCommand(CMD_PROM + (i * 2));
    Wire.requestFrom( _address, 2);
    uint8_t highByte = Wire.read();
    uint8_t lowByte = Wire.read();
    coefficient[i] = (highByte << 8)|lowByte;
    // Uncomment below for debugging output.
    //	Serial.print("C");
    //	Serial.print(i);
    //	Serial.print("= ");
    //	Serial.println(coefficient[i]);
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

  return pressure_reported;
}

void MSxxxx::sendADCCommand(measurement _measurement, precision _precision) {
    sendCommand(CMD_ADC_CONV + _measurement + _precision);
}

uint32_t MSxxxx::readRawTemp() {
  // Reads temperature value from the bus and sets variable
  _temperature_raw = readValue();
}

uint32_t MSxxxx::readRawPressure() {
  // Read the raw pressure value from the bus and sets variable
  _pressure_raw = readValue();
}

// Note - when calling after sendADCCommand, wait to read from ADC until
// appropriate ADC conversion time has passed (see datasheet, resolution-dependent)
uint32_t MSxxxx::readValue() {
  uint32_t result;
  uint8_t highByte, midByte, lowByte;
  sendCommand(CMD_ADC_READ);
  Wire.requestFrom(_address, 3);

  while(Wire.available())
  {
    highByte = Wire.read();
    midByte = Wire.read();
    lowByte = Wire.read();
  }

  result = ((uint32_t)highByte << 16) + ((uint32_t)midByte << 8) + lowByte;

  return result;
}

uint32_t MSxxxx::getADCconversion(measurement _measurement, precision _precision)
// Retrieve ADC measurement from the device.
// Select measurement type and precision
{
  uint32_t result;
  uint8_t highByte, midByte, lowByte;

  sendCommand(CMD_ADC_CONV + _measurement + _precision);
  // Wait for conversion to complete
  sensorWait(1); //general delay
  switch( _precision )
  {
    case ADC_256 : sensorWait(1); break;
    case ADC_512 : sensorWait(3); break;
    case ADC_1024: sensorWait(4); break;
    case ADC_2048: sensorWait(6); break;
    case ADC_4096: sensorWait(10); break;
  }

  sendCommand(CMD_ADC_READ);
  Wire.requestFrom(_address, 3);

  while(Wire.available())
  {
    highByte = Wire.read();
    midByte = Wire.read();
    lowByte = Wire.read();
  }

  result = ((uint32_t)highByte << 16) + ((uint32_t)midByte << 8) + lowByte;

  return result;

}

void MSxxxx::sendCommand(uint8_t command)
{
  Wire.beginTransmission( _address);
  Wire.write(command);
  Wire.endTransmission();

}

void MSxxxx::sensorWait(uint8_t time)
// Delay function.  This can be modified to work outside of Arduino based MCU's (Photon included compatible delay function)
{
  delay(time);
};
