
/******************************************************************************
twoCh_I2CPressureToDAC_noTimer.ino
Combines AdaFruit and SparkFun demo code with improvements and modifications to
output the read values over DAC

I2C - 10k pull up resistors on SCL/SDA
//SDA -- D0
//SCL -- D1

Licensed under GPL v3


This version uses simple delay() calls to handle the timing between loop
iterationsI2C read and also writes the  pressure value over DAC


Original SF license --
This code is beerware. If you see me (or any other SparkFun employee) at the
local pub, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/
//#include "SparkIntervalTimer.h"
//#include "SparkFun_MS5803_I2C.h"
//#include "MS5607_I2C.h"
#include "MSxxxx_I2C.h"


#define CONSOLEDEBUG 1
#define FS 70


#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

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


    //AJO TO-DO -- make sure each sensor is inited correctly?

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

  // Convert mbar to voltage in DAC range (0-4095 for 12-bit Photon DAC)
  //p_ms5607_V = (p_abs_ms5607 - BMP388_MBAR_LOW) * 1.0/BMP388_MBAR_RANGE * 4095;
  p_rel_analog_ms5607 = (p_abs_ms5607 - p_baseline_ms5607) * 1.0/MS5607_MBAR_RANGE_UKLIMIT * 4095;
  p_rel_analog_ms5803 = (p_abs_ms5803 - p_baseline_ms5803) * 1.0/MS5803_MBAR_RANGE_UKLIMIT * 4095;

  // Note -- does Photon lib automatically limit this if it's out of range?
  if(p_rel_analog_ms5607 > 4095) p_rel_analog_ms5607 = 4095;
  if(p_rel_analog_ms5803 > 4095) p_rel_analog_ms5803 = 4095;


  //Output over DAC
  //AJO TO-DO -- change ordering here -- read, set DAC; read, set DAC?
  analogWrite(DAC1, p_rel_analog_ms5607); // MS5607 --> p_sys
  analogWrite(DAC2, p_rel_analog_ms5803); // MS5803 --> p_res

  if(CONSOLEDEBUG) {
    Serial.print("Time = ");
    Serial.print(stepTime);
    Serial.println(" ms");

    Serial.print("MS5803 Pressure = ");
    Serial.print(p_abs_ms5803);
    Serial.println(" hPa");

    Serial.print("MS5803 Output signal = ");
    Serial.print(double(p_rel_analog_ms5803)/4095 * 3.3);
    Serial.println(" V");

    Serial.print("MS5607 Pressure = ");
    Serial.print(p_abs_ms5607);
    Serial.println(" hPa");

    Serial.print("MS5607 Output signal = ");
    Serial.print(double(p_rel_analog_ms5607)/4095 * 3.3);
    Serial.println(" V");
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
