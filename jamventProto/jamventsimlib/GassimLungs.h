#ifndef __GASSIMLUNGS_H__
#define __GASSIMLUNGS_H__

#include "GassimNode.h"

class GassimLungs : public GassimNode {
public:
	GassimLungs(const std::string &name, 
		double pressure, 
		double volume,
		double compliance);
	virtual void step(double dt);
	virtual void  setPo2(double pO2);

protected:

private:

};
#endif
