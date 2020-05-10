#ifndef __GASSIM_MODEL__H__
#define __GASSIM_MODEL__H__


#include <map>
#include "GassimNode.h"
#include "GassimConst.h"
#include "GassimConv.h"
#include "GassimPipe.h"
#include "GassimSource.h"
#include "GassimReservoir.h"
#include "GassimLungs.h"


class GassimModel {
public:
	GassimModel()
		: _nextId(0)
		, _steps(0) {}
	;
	virtual ~GassimModel() {}
	;

	virtual void connect(GassimNode::NodePtr_t &a, GassimNode::NodePtr_t &b);
	virtual void addNode(GassimNode::NodePtr_t nodePtr);
	void init();				            // first step initialization.
	virtual void step(double dt);			// step the simulation for delta time.
	virtual void next();					// proceed to the next step...

protected :
private :

	std::map<unsigned, GassimNode::NodePtr_t> _childNodes;			// 
	unsigned _nextId;		// 
	unsigned _steps;
};

#endif

