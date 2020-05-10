#ifndef __GASSIM_VARS_H__
#define __GASSIM_VARS_H__

#include "GassimConv.h"

// container class of the gas sim vars...
// currently it is a super set of the data
// 
class GassimVars {
public:
	double pressure;		// pressure in bars
	double volume;			// voume in liters
	double temp;			// temperature in Kelvin
	double pO2;				// fraction O2 0..1.0
	double flow;			// flow in mols per second.
	double resistance;		// resistance presDrop/liter/s
	double compliance;		// compliance of a flexable container.
	bool   open;			// open or closed (turn a pipe into a valve)
	GassimVars() {
		pressure = 1.0;
		volume = 1.0;
		temp = GassimConv::cToKelvin(21);
		pO2 = .21;
		resistance=0;
		compliance=0;			// compiance, 0 is rigid container.
		open=true;
	};


	

};


#endif
