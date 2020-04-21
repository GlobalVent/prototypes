
#include <assert.h>
#include <math.h>
#include "GassimPipe.h"



GassimPipe::GassimPipe(const std::string &name, 
			   double resistance,
			   bool open) : GassimNode(name, PIPE) {
	setResistance(resistance);
	setOpen(open);
}

void GassimPipe::step(double dt) {
	assert(getNumConnections() == 2);		// todo do something better than assert.
	ConnectionMap_t::iterator p  = _connections.begin();
	NodePtr_t pA = p->second; p++;		// extract the first 2 connections...
	NodePtr_t pB = p->second; p++;
	double pdrop = abs(pA->pressure() - pB->pressure());
	if (open())
		setFlow(pdrop / resistance());
	else
		setFlow(0);
}		
double GassimPipe::getPressureDrop(unsigned reqNodeId) {
	ConnectionMap_t::iterator p  = _connections.begin();
	NodePtr_t pA = p->second; p++;		// extract the first 2 connections...
	NodePtr_t pB = p->second; p++;
	double pdrop = abs(pA->pressure() - pB->pressure());
	if (open()) {
		if (nodeId() == reqNodeId)
			pdrop = -pdrop;   // flowing away...
	}
	else 
		pdrop = INFINITY;
	return(pdrop);
}





