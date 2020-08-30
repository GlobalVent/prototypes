#ifndef __VALVES_H__
#define __VALVES_H__

#include <string>
#include <map>
#include <pigpio.h>

class Valves {
public:
	Valves() {};

	// individual valves to control
	enum Valve_t {
		VALVE_A_O2,
		VALVE_B_AIR,
		VALVE_C_INHALE,
		VALVE_D_EXHALE,
	};
	
	int init();
	int setState(Valve_t valve, bool state);
	
protected:
	// GPIO pins that control each valve
	static const std::map<unsigned, unsigned> VALVE_GPIOS;
};

#endif
