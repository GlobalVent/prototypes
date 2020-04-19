

#include <assert.h>
#include <math.h>
#include "GassimLungs.h"


GassimLungs::GassimLungs(const std::string &name, 
		double pressure, 
		double volume,
        double compliance) : GassimNode(name, LUNGS) {
	setPressure(pressure);
	setVolume(volume);
    setCompliance(compliance);
}
void  GassimLungs::setPo2(double pO2) {
	assert(pO2 <= 1.0);				// double check...
    assert (pO2 > 0);
	GassimLungs::setPressure(pO2);
};

void GassimLungs::step(double dt) {        // # avoid dealing with both the pressure AND temperature chaning

    // # avoid dealing with both the pressure AND temperature chaning
    // #    I am assuming an adbadiac conditions, to avoid a having to
    // #      solve 2 ordanary differential equations...

    // # keep this simple for each device connected to us for now
    // #   assume a single source formula instead of parallel
    // #   sources from different pressures...

    // # uber simple lung model, RC cirucit analog
    // #
    // #   ----- R ------
    // #                 |
    // #                 |
    // #                ---
    // #                ---    C Compliance (use the volume of the container)
    // #                 |
    // #                 |
    // #              -------
    // #                ---
    // #                 -
    // #
    // # we can always make the model more
    // #    complicated if we need it.
    // 
    // 
    // TODO work in compliance and a more complicated model
    //   for now this will do...
    unsigned numValveOpen=0;
    double pDelta =0;
    double ppNewO2=0;
    for (auto citer : _connections) {
    	NodePtr_t c = citer.second;
        if (! c->open())
        	continue;
        numValveOpen += 1;
        // #
        // #              1                # calculate the new pressure..
        // # P      = (-----------)        #   this is only valid for a single
        // #               dt/RV           #   inlet at a time... no parallel inlets...
        // #             e
        double pDrop = c->getPressureDrop(nodeId());	// pressure drop relative to me...
        double R=c->resistance();
        double V=volume();
	    //double tc=1-exp(-dt/(R*V))
	    pDelta = pDrop*(1 - exp(-dt / (R*V)));      //# this is not handling multiple circuits open.
        ppNewO2+=pDelta*(c->pO2());
    }

    assert(numValveOpen <= 1);      //# don't support parallel filling of the container yet...
    // # try working with partial pressures.
    double ppO2 = pressure() * pO2();
    ppO2 += ppNewO2;
    setPressure(pressure()+pDelta);
    setPo2(ppO2/pressure());
}
