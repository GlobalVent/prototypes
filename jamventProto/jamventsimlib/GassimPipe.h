#ifndef __GASSIMPIPE_H__
#define __GASSIMPIPE_H__

#include "GassimNode.h"

class GassimPipe : public GassimNode {
public:	
	GassimPipe() = delete;
	GassimPipe(const std::string &name, 
		double pressure,
		double resistance,
		bool open=true);
	virtual void step(double dt);			
	virtual double getPressureDrop(unsigned reqNodeId);

protected:
private:
};


#endif
