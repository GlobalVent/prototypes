/*
 * Project photonjamsim
 * Description:
 * Author:
 * Date:
 */


/**
 * @file photonjamsim.ino
 * @author 
 * @brief phonton jamvent simulator.
 * @version 0.1
 * @date 2020-07-26
 * 
 * @copyright Copyright (c) 2020
 * 
 * this module simulates the jamvent i2c devices.
 * specifically it simulates the pressure sensors and O2 sensors for the 
 * jamven/globalvent design
 * 
 * The simulator emulates the actual hardware...
 * 
 * 
 */

#include "I2cIntHandler.h"
#include "I2cSlaveCtl.h"
#include "I2cJamsimConfig.h"
#include "I2cMS5607Sim.h"
#include "I2cMS5803Sim.h"
#include "I2cAdcSensor.h"
#include "JamventTime.h"
#include "JamsimDbgPrint.h"
#include "JamventSimModelRt.h"

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(MANUAL);


// gpio defs  (these will change with our proto board.)
#define I2C_RPI_SCL   A1
#define I2C_RPI_SDA   A4
#define I2C_CPLD_SCL  D3
#define I2C_CPLD_SDA  D2

#define GPIO_VALVEA D7
#define GPIO_VALVEB D6
#define GPIO_VALVEC D5
#define GPIO_VALVED D4

// stay away from these addresses
#define I2C_PRES_ADDR 0x76  // check these against the specs...
#define I2C_PSYS_ADDR 0x77

// our simulation addresses...
#define I2C_PHOTON_CFG_ADDR 0x60
#define I2C_PRES_SIM_ADDR   0x61
#define I2C_PSYS_SIM_ADDR   0x62
#define I2C_ADC_SIM_ADDR    0x63

static I2cIntHandler *i2cIntHandler = nullptr;
static I2cSlaveCtl *i2cRpiSlaveCtl = nullptr;
static I2cSlaveCtl *i2cCpldSlaveCtl = nullptr;
static I2cJamsimConfig *photonConfig;
static I2cMS5803Sim *i2cRpiPresSensor;
static I2cMS5803Sim *i2cCpldPresSensor;
static I2cMS5607Sim *i2cRpiPsysSensor;
static I2cMS5607Sim *i2cCpldPsysSensor;
static JamventSimModelRt *gasModel;


static double timeToPrint = 0;
static JamventTime jamTime;
static JamsimDbgPrint dbgPrint;
double lastTime = 0;			// last time we got called


int counter = 0;

void initialize() {
	static bool isInitialized = false;
	if (isInitialized)
		return;


	pinMode(GPIO_VALVEA, INPUT);
	pinMode(GPIO_VALVEB, INPUT);
	pinMode(GPIO_VALVEC, INPUT);
	pinMode(GPIO_VALVED, INPUT);


	gasModel = new JamventSimModelRt();
	gasModel->init();				// time zero state.
	gasModel->setPressDly(0);		// not delay for pressure
	gasModel->setO2Dly(0);
	lastTime = jamTime.now();

	i2cIntHandler = new I2cIntHandler();
	i2cIntHandler->setDbgPrint(&dbgPrint);
	i2cRpiSlaveCtl = new I2cSlaveCtl(I2C_RPI_SCL, I2C_RPI_SDA);
	photonConfig = new I2cJamsimConfig(I2C_PHOTON_CFG_ADDR);
	i2cRpiPresSensor = new I2cMS5803Sim(I2C_PRES_ADDR);
	i2cRpiPsysSensor = new I2cMS5607Sim(I2C_PSYS_ADDR);


	i2cCpldSlaveCtl = new I2cSlaveCtl(I2C_CPLD_SCL, I2C_CPLD_SDA);
	i2cCpldPresSensor = new I2cMS5803Sim(I2C_PRES_ADDR);
	i2cCpldPsysSensor = new I2cMS5607Sim(I2C_PSYS_ADDR);

	// wire it all up.
	i2cIntHandler->registerI2cSlaveCtl(i2cRpiSlaveCtl);		// two slave controllers
	i2cIntHandler->registerI2cSlaveCtl(i2cCpldSlaveCtl);

	i2cRpiSlaveCtl->registerI2cDevice(photonConfig);		// rpi sensors
	i2cRpiSlaveCtl->registerI2cDevice(i2cRpiPresSensor);

	i2cCpldSlaveCtl->registerI2cDevice(i2cCpldPresSensor);	// cpld sensors

	i2cRpiSlaveCtl->setDbgPrint(&dbgPrint);
	i2cCpldSlaveCtl->setDbgPrint(&dbgPrint);

	i2cIntHandler->attachAllInterrupts();

	isInitialized=true;

}

void setup() {
	initialize();
}	


void loop() {

	return;		// temporarly bypass all of this...

	if (i2cIntHandler == nullptr)
		return;

	gasModel->setValveAopen(pinReadFast(GPIO_VALVEA));
	gasModel->setValveBopen(pinReadFast(GPIO_VALVEB));
	gasModel->setValveCopen(pinReadFast(GPIO_VALVEC));
	gasModel->setValveDopen(pinReadFast(GPIO_VALVED));

	i2cIntHandler->microsNow();			// keep this refreshed so we have an accurate micros count...
	double now = jamTime.now();
	//i2cSlaveCtl->sampleIO();
	double dt = now - lastTime;
	double simInterval = (double)photonConfig->getSimInterval()/1000.0;
	if (dt >= simInterval) {			// hit the model at 1ms intervals.
		gasModel->step(dt);
		if (!photonConfig->getLoopBack()) {
			i2cRpiPresSensor->setPressure(gasModel->getPres());
			i2cCpldPresSensor->setPressure(gasModel->getPres());
			i2cRpiPsysSensor->setPressure(gasModel->getPsys());
			i2cCpldPsysSensor->setPressure(gasModel->getPsys());
			// todo, add the o2 sensor...
		}
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
