#ifndef __GASSIMSOURCE_H__
#define __GASSIMSOURCE_H__


#include "GassimNode.h"
class GassimSource : public GassimNode 
{
public:
	GassimSource() = delete;
	GassimSource(const std::string &name, 
		double pressure, 
		double pO2);
	void step(double dt) {};		// nothing to do for source

protected:
private:

};


#endif
