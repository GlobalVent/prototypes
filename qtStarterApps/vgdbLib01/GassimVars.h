#ifndef __GASSIM_VARS_H__
#define __GASSIM_VARS_H__

//#
//# container of gas variables.

//# container with gas variables for use in the simulation...
//# these will be the inputs, the outputs and the new outputs.
//# These are variables of ALL the variables that can be set for any
//# given node.

//# container of every possible variable we could use...

#include "GassimConst.h"
#include "GassimConv.h"

// public container class for manulipating gas variables.
class GassimVars
{
public:
	GassimVars()
	{
		pressure = 1;	// 1 bar
		volume = 1;		// 1 liter
		temp = GassimConv::cToKelvin(21);	// room temperature
		pO2 = 21;							// air percent
		flow = 0;
		resistance = 0;
		open = true;
	};
	double pressure;    // pressure in bars
	double volume;      // current volume in liters
	double temp;        // current temperature (kelvin)
	double pO2;         // percent O2
	double flow;        // flow in l/s
	double resistance;  // resistance to flow
	bool open;          // true if valve is open, closed if not.
	
protected:
	
private:
	
};
    
#endif
