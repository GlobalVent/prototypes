
/******************************************************************************
MSxxxx_I2C.h
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

#ifndef SparkFun_MSxxxx_I2C_h
#define SparkFun_MSxxxx_I2C_h

#include <Arduino.h>

 // Already defined in MS5803 headers
// Define units for conversions.
enum temperature_units
{
	CELSIUS,
	FAHRENHEIT,
};

// Define measurement type.
enum measurement
{
	PRESSURE = 0x00,
	TEMPERATURE = 0x10
};

// Define constants for Conversion precision
enum precision
{
	ADC_256  = 0x00,
	ADC_512  = 0x02,
	ADC_1024 = 0x04,
	ADC_2048 = 0x06,
	ADC_4096 = 0x08
};

// Define address choices for the device (I2C mode)
enum ms_addr
{
	MS_ADDRESS_HIGH = 0x76,
	MS_ADDRESS_LOW  = 0x77
};

enum ms_model
{
	MS5803 = 0x00,
	MS5607 = 0xFF
};

//Commands
#define CMD_RESET 0x1E // reset command
#define CMD_ADC_READ 0x00 // ADC read command
#define CMD_ADC_CONV 0x40 // ADC conversion command

#define CMD_PROM 0xA0 // Coefficient location


class MSxxxx
{
	public:
		MSxxxx(ms_addr address, ms_model);
		void reset(void);	 //Reset device
		uint8_t begin(void); // Collect coefficients from sensor

		// Return calculated temperature or pressure from sensor
		// These 2 functions using a blocking delay inline to wait for the ADC
		// conversion time specified in the data sheet
		float getTemperature(temperature_units units, precision _precision);
		float getPressure(precision _precision);

		// Use following 4 functions to send the ADC conversion command separately
		// from reading back+applying temperature compensation
		// 1. Send ADC conversion cmd only, allows calling function to do something else
		// in meantime of blocking and reading all in one function (use for multiple sensor reads)
		void sendADCCommand(measurement _measurement, precision _precision);
		uint32_t readRawPressure();
		uint32_t readRawTemp();
		float convertRawValues();


	private:

		int32_t _temperature_raw;
		int32_t _pressure_raw;

		int32_t _temperature_actual;
		int32_t _pressure_actual;

		ms_model _ms_model;
		ms_addr _address; 		// Variable used to store I2C device address.
		uint16_t coefficient[8];// Coefficients;

		uint32_t readValue(); //2. Once data are ready, read
		void sendCommand(uint8_t command);	// General I2C send command function
		uint32_t getADCconversion(measurement _measurement, precision _precision);	// Retrieve ADC result

		void sensorWait(uint8_t time); // General delay function see: delay()
};

#endif
