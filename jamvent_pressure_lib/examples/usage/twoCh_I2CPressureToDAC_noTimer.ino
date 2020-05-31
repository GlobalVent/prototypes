
/******************************************************************************
twoCh_I2CPressureToDAC_noTimer.ino
Author: AJ Ortiz
Organization: GlobalVent
Date: 2020
License: GPL V3

Demo of MSxxxx_I2C library functions to break out the different steps of a
pressure sensor read. This allows us to do the ADC conversion time wait for
multiple sensors at once instead of asking one for data, waiting for Tadc and
then repeating all of this for the other (effectively almost twice as slow).

1. Send read temperature command to each of the 2 sensors
2. Wait for ADC conversion
3. Read back raw value from each sensor
4. Repeat 1-3 for pressure
5. Calculate actual pressure using raw values and PROM coefficients

I2C - 10k pull up resistors on SCL/SDA
//SDA -- D0
//SCL -- D1

This version uses no timing between loop iterations, the blocking delays for
ADC conversion of temp and pressure will determine how quickly it runs. The
period between each loop iteration is printed to the console for reference -
~19 ms if sampling both temperature and pressure at ADC_2048 on each loop


******************************************************************************/
#include "MSxxxx_I2C.h"


#define CONSOLEDEBUG 1
#define DAC_OUTPUT 1
#define FS 70 // Note -- currently unused with loop running as quickly as possible

// Both sensors use same address scheme, need to set CS pin for one of them
MSxxxx p_sys_sensor(MS_ADDRESS_LOW, MS5607); //
MSxxxx p_res_sensor(MS_ADDRESS_HIGH, MS5803); // ADDRESS_HIGH is part of library

int MS5607_MBAR_HIGH = 1250;
int MS5607_MBAR_LOW = 300;
int MS5607_MBAR_RANGE = MS5607_MBAR_HIGH - MS5607_MBAR_LOW;
int MS5607_MBAR_RANGE_UKLIMIT = 140;


int MS5803_MBAR_RANGE = 5000;
int MS5803_MBAR_RANGE_UKLIMIT = 1000;

void readSensorsFn(void);
void debugStartPulse(void);

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

void setup() {

  //Pin inits
  pinMode(DAC1, OUTPUT);
  pinMode(DAC2, OUTPUT);

  Serial.begin(115200);
  while (!Serial);
  Serial.println("MS5607 and MS5803 --> Read and output over DAC pins (Range-limited to match UK sensors)");

  // Compare both resolutions to see which one is higher, this will determine
  // the ADC conversion time (assuming equal sample rate for both sensors)
  if(p_res_resolution >= p_sys_resolution) {
    max_res = p_res_resolution;
  } else {
    max_res = p_sys_resolution;
  }

  p_res_sensor.reset();
  p_res_sensor.begin();
  delay(1000);

  p_sys_sensor.reset();
  p_sys_sensor.begin();


  // Read pressure from the sensors in mbar (temp correction applied inline)
  // AJO TO-DO - Do multiple readings here and average?
  p_baseline_ms5607 = p_sys_sensor.getPressure(ADC_4096);
  p_baseline_ms5803 = p_res_sensor.getPressure(ADC_4096);
  p_baseline_ms5607 = p_sys_sensor.getPressure(ADC_4096);
  p_baseline_ms5803 = p_res_sensor.getPressure(ADC_4096);

  if(CONSOLEDEBUG) {
    Serial.print("MS5607 baseline pressure = ");
    Serial.print(p_baseline_ms5607);
    Serial.println(" mbar");

    Serial.print("MS5803 baseline pressure = ");
    Serial.print(p_baseline_ms5803);
    Serial.println(" mbar");
  }
  debugStartPulse();
}


void loop() {
  stepTime = millis() - startTime;
  readSensorsFn();
  // AJO TO-DO -- add logic to subtract elapsed time from Ts_millis and only
  // delay for the remaining time left, at ADC_2048 can't go more than 14 ms
  // or 71 Hz. With no delay @ ADC_2048 for both we get Tloop = ~19 ms
  //  delay(Ts_millis);
  
  if(CONSOLEDEBUG) {
    Serial.print("Time = ");
    Serial.print(stepTime);
    Serial.println(" ms");
  }
}

void readSensorsFn() {
  // 1. send adc cmd (pressure) for each sensor
  // 2. delay by higher of 2 ADC settling times
  // 3. read values
  // 4. repeat 1-3 for temperature
  // 5. Convert to compensated/actual values

  p_sys_sensor.sendADCCommand(TEMPERATURE, p_sys_resolution);
  p_res_sensor.sendADCCommand(TEMPERATURE, p_res_resolution);


  // AJO note -- faster if we predefine delay during setup based on resolutions?
  delay(1); //general delay
  switch(max_res)
  {
    case ADC_256 : delay(1); break;
    case ADC_512 : delay(3); break;
    case ADC_1024: delay(4); break;
    case ADC_2048: delay(6); break;
    case ADC_4096: delay(10); break;
  }


  p_sys_sensor.readRawTemp();
  p_res_sensor.readRawTemp();

  // Read pressure from the sensor in mbar (temp correction applied inline)
  p_sys_sensor.sendADCCommand(PRESSURE, p_sys_resolution);
  p_res_sensor.sendADCCommand(PRESSURE, p_res_resolution);

  // AJO note -- faster if we predefine delay during setup based on resolutions?
  delay(1); //general delay
  switch(max_res)
  {
    case ADC_256 : delay(1); break;
    case ADC_512 : delay(3); break;
    case ADC_1024: delay(4); break;
    case ADC_2048: delay(6); break;
    case ADC_4096: delay(10); break;
  }


  p_sys_sensor.readRawPressure();
  p_res_sensor.readRawPressure();

  //Calculate actual values
  p_abs_ms5607 = p_sys_sensor.convertRawValues();
  p_abs_ms5803 = p_res_sensor.convertRawValues();

  if(CONSOLEDEBUG) {
    Serial.print("MS5803 Pressure = ");
    Serial.print(p_abs_ms5803);
    Serial.println(" hPa");

    Serial.print("MS5607 Pressure = ");
    Serial.print(p_abs_ms5607);
    Serial.println(" hPa");
  }


  if(DAC_OUTPUT) {
    // Convert mbar to voltage in DAC range (0-4095 for 12-bit Photon DAC)
    p_rel_analog_ms5607 = (p_abs_ms5607 - p_baseline_ms5607) * 1.0/MS5607_MBAR_RANGE_UKLIMIT * 4095;
    p_rel_analog_ms5803 = (p_abs_ms5803 - p_baseline_ms5803) * 1.0/MS5803_MBAR_RANGE_UKLIMIT * 4095;

    // Note -- does Photon lib automatically limit this if it's out of range?
    if(p_rel_analog_ms5607 > 4095) p_rel_analog_ms5607 = 4095;
    if(p_rel_analog_ms5803 > 4095) p_rel_analog_ms5803 = 4095;

    //Output over DAC
    analogWrite(DAC1, p_rel_analog_ms5607); // MS5607 --> p_sys
    analogWrite(DAC2, p_rel_analog_ms5803); // MS5803 --> p_res

    if(CONSOLEDEBUG) {
      Serial.print("MS5607 Output signal = ");
      Serial.print(double(p_rel_analog_ms5607)/4095 * 3.3);
      Serial.println(" V");

      Serial.print("MS5803 Output signal = ");
      Serial.print(double(p_rel_analog_ms5803)/4095 * 3.3);
      Serial.println(" V");
    }
  }
}



void debugStartPulse() {
  //Gives us some entries of known amplitudes to mark start of file in debug stream and on scope (to align later and compare signals)
  int i;
  startTime = millis();
  for(i=0; i < 30; i++) {

    analogWrite(DAC1, 4095);
    analogWrite(DAC2, 4095);

    Serial.print("MS5803 Output signal = ");
    Serial.println("3.30 V");

    Serial.print("MS5607 Output signal = ");
    Serial.println("3.30 V");

    delay(Ts_millis);

    analogWrite(DAC1, 0);
    analogWrite(DAC2, 0);

    Serial.print("MS5803 Output signal = ");
    Serial.println("0.00 V");

    Serial.print("MS5607 Output signal = ");
    Serial.println("0.00 V");

    delay(Ts_millis);
  }

}
