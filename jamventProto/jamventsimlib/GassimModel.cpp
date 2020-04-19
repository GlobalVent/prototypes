
/**
 * GassimModel.  Create one of these per simulator...
 * 
 */

#include "GassimModel.h"


void GassimModel::addNode(GassimNode::NodePtr_t nodePtr) {
	unsigned nodeId = ++_nextId;
	nodePtr->setNodeId(nodeId);
	_childNodes[nodeId] = nodePtr;
}

void GassimModel::connect(GassimNode::NodePtr_t &a, GassimNode::NodePtr_t &b) {
	a->connect(b);
	b->connect(a);
}

void GassimModel::step(double dt) {
	if (_steps++ == 0) 			// very first iteration call next first...
		next();
	
	for (auto n : _childNodes) 
		n.second->step(dt);
	
}			// step the simulation for delta time.
void GassimModel::next() {
	for (auto n : _childNodes) 
		n.second->next();
}

