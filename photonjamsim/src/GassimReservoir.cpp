

#include <math.h>

#include "GassimReservoir.h"

using namespace std;

GassimReservoir::GassimReservoir(const std::string &name, 
		double pressure, 
		double volume) : GassimNode(name, RESERVOIR) {
	setPressure(pressure);
	setVolume(volume);
}
void  GassimReservoir::setPO2(double pO2) {
	GassimNode::setPO2(pO2);
};

void GassimReservoir::step(double dt) {



	// # avoid dealing with both the pressure AND temperature chaning
	// #    I am assuming an adbadiac conditions, to avoid a having to
	// #      solve 2 ordanary differential equations...

	// # keep this simple for each device connected to us for now
	// #   assume a single source formula instead of parallel
	// #   sources from different pressures...

	// # for each connection figure out how much is entering or leaving in mols
    unsigned numValveOpen=0;
    double pDelta=0;
    double ppNewO2=0;
    for (auto citer : _connections) {
    	NodePtr_t c = citer.second;
        if (! c->open())
        	continue;
        numValveOpen += 1;
        if (numValveOpen >= 2)      // punt if we have more than one open.
            break;
        // #
        // #                -t                # calculate the new pressure..
        // # P      = 1- (-----------)        #   this is only valid for a single
        // #                  dt/RV           #   inlet at a time... no parallel inlets...
        // #                e
        double pDrop = c->getPressureDrop(nodeId());	// pressure drop relative to me...

        double R=c->resistance();
        double V=volume();
	    double tc=1-exp(-dt/(R*V));
	    pDelta += pDrop*(1 - exp(-dt / (R*V)));      //# this is not handling multiple circuits open.
        if (pDrop > 0)                               // our percent Po2 only changes if we are gaining gas
            ppNewO2+=pDelta*(c->pO2());
	}

    // with more than one valve open, it results won't quite be correct...
    // # ttry working with partial pressures.
    double ppO2 = pressure() * pO2();
    double newP = pressure() + pDelta;
    setPressure(newP);
    if (ppNewO2) {          // no change, so don't recalculate...
        ppO2 += ppNewO2;
        setPO2(ppO2/newP);
    }
}
