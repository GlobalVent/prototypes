/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Volumes/ct-ng/covid-19/github/prototypes/photonjamsim/src/photonjamsim.ino"
/*
 * Project photonjamsim
 * Description:
 * Author:
 * Date:
 */

#include "I2cSlaveCtl.h"
#include "I2cPhotonJamsimConfig.h"

// gpio defs
void setup();
void loop();
#line 12 "/Volumes/ct-ng/covid-19/github/prototypes/photonjamsim/src/photonjamsim.ino"
#define I2C_RPI_SDL D4
#define I2C_RPI_SDA D5


// stay away from these addresses
#define I2C_PRES_ADDR_A 0x76
#define I2C_PRES_ADDR_B 0x77

#define I2C_PHOTON_CONFIG_ADDR 0b0000110

I2cSlaveCtl *i2cSlaveCtl = nullptr;
I2cPhotonJamsimConfig *photonConfig;



int counter = 0;


void setup() {
	Serial.begin(9600);			// debugger no workee we need to do this the old fashion way...
	i2cSlaveCtl = new I2cSlaveCtl(I2C_RPI_SDL, I2C_RPI_SDA);
	photonConfig = new I2cPhotonJamsimConfig(I2C_PHOTON_CONFIG_ADDR);
	

}	


void loop() {
	i2cSlaveCtl->sampleIO();
}
