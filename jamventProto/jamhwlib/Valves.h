#ifndef __VALVES_H__
#define __VALVES_H__

#include <string>
#include <map>
#include <pigpio.h>

class Valves {
public:
	// individual valves to control
	enum Valve_t {
		VALVE_A_O2,
		VALVE_B_AIR,
		VALVE_C_INHALE,
		VALVE_D_EXHALE,
	};
	
	static int init();
	static int setState(Valve_t valve, bool state);
	
protected:
	// GPIO pins that control each valve
	using GpioMap_t = std::map<Valve_t, unsigned>;
	static const GpioMap_t VALVE_GPIOS;
};

#endif
