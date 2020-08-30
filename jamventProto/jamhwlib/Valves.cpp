#include "Valves.h"

// mapping from valve names to GPIO pins
const std::map<unsigned, unsigned> Valves::VALVE_GPIOS = { { VALVE_A_O2, 21 }, { VALVE_B_AIR, 20 }, { VALVE_C_INHALE, 16 }, { VALVE_D_EXHALE, 12 } };

/**
 * @brief Initializes GPIOs for valve outputs, with all valves off by default
 *        
 * @return int -- 0 on success, pigpio error code on error
 */
int Valves::init() {
	int rc;
	
	// set GPIO pins as outputs
	std::map<unsigned, unsigned>::const_iterator it = VALVE_GPIOS.begin();
	for(std::pair<unsigned, unsigned> it : VALVE_GPIOS) {
		rc = gpioSetMode(it.second, PI_OUTPUT);
		if (rc != 0) return rc;
		rc = gpioSetPullUpDown(it.second, PI_PUD_OFF);
		if (rc != 0) return rc;
		rc = gpioWrite(it.second, 0);
		if (rc != 0) return rc;
	}
	
	return 0;
}

/**
 * @brief Sets the state of a valve
 *        
 * @param valve -- valve to control (one of VALVE_A, VALVE_B, VALVE_C, or VALVE_D)
 * @param state -- true to activate, false to de-activate (close a normally-closed valve, or open a normally-open valve)
 * @return int -- 0 on success, pigpio error code on error
 */
int Valves::setState(Valve_t valve, bool state) {
	// check valve number
	if (VALVE_GPIOS.count(valve) == 0)
		return PI_BAD_PARAM;

	unsigned const gpioPin = VALVE_GPIOS.at(valve);
	return gpioWrite(gpioPin, (unsigned int)state);
}
