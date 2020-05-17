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
	virtual void  setPO2(double pO2);

protected:
	// double complianceFactor(double compliance, 
	// 						double deltaV, 
	// 						double deltaP);
	typedef std::pair<double,double> VP_t;  // volume pressure pair..
	VP_t intersect(VP_t A, VP_t B, VP_t C, VP_t D);

private:

};
#endif
