

#include "GassimSource.h"


GassimSource::GassimSource(const std::string &name, 
				double pressure, 
				double pO2) : GassimNode(name, GASSOURCE) {
	setPressure(pressure);
	setPO2(pO2);
}


