/*
 * Project photonjamsim
 * Description:
 * Author:
 * Date:
 */

#include "I2cSlaveCtl.h"
#include "I2cPhotonJamsimConfig.h"
#include "JamventTime.h"
#include "JamsimDbgPrint.h"

// gpio defs  (these will change with our proto board.)
#define I2C_RPI_SDL D5
#define I2C_RPI_SDA D4

#define GPIO_VALVEA D0
#define GPIO_VALVEB D1
#define GPIO_VALVEC D2
#define GPIO_VALVED D3


// stay away from these addresses
#define I2C_PRES_ADDR_A 0x76
#define I2C_PRES_ADDR_B 0x77

#define I2C_PHOTON_CONFIG_ADDR 0b0000110

I2cSlaveCtl *i2cSlaveCtl = nullptr;
I2cPhotonJamsimConfig *photonConfig;
static double timeToWake = 0;
static double timeToPrint = 0;
static JamventTime jamTime;
static JamsimDbgPrint dbgPrint;


int counter = 0;

void setup() {
	Serial.begin(9600);			// debugger no workee we need to do this the old fashion way...
	i2cSlaveCtl = new I2cSlaveCtl(I2C_RPI_SDL, I2C_RPI_SDA);
	i2cSlaveCtl->setDbgPrint(&dbgPrint);
	photonConfig = new I2cPhotonJamsimConfig(I2C_PHOTON_CONFIG_ADDR);
	//photonConfig->setLogStream(&log);
	pinMode(D7,OUTPUT);


}	


void loop() {

	double now = jamTime.now();
	i2cSlaveCtl->sampleIO(now);
	if (now >= timeToWake) {
		// call the simulation.

		// very nice.... next....
		timeToWake = now + photonConfig->getSimInterval()*1e-3;	// convert to milliseconds
	}
	if (now >= timeToPrint) {
		if (dbgPrint.hasData()) {
			Serial.print(dbgPrint.getPrintBuffer());
			dbgPrint.clear();
		}
		timeToPrint += 1.0;
	}
	#if 0
	static unsigned cnt = 0;
	cnt++;
	//Serial.print("setting A0 high\n");
	digitalWrite(D7, (cnt&1));
	delay(1000);
	//digitalWrite(A0,HIGH);
	#endif
}
