#ifndef __GassimReservoir_H__
#define __GassimReservoir_H__

#include "GassimNode.h"

class GassimReservoir : public GassimNode {
public:
	GassimReservoir(const std::string &name, 
		double pressure, 
		double volume);
	virtual void step(double dt);

	virtual void   setPO2(double pO2);

protected:

private:

};
#endif

