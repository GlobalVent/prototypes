

#include <assert.h>
#include "GassimNode.h"

GassimNode::GassimNode(const std::string &name, NodeType nodeType) :
  _name(name),
  _nodeType(nodeType) {


}
	// called once by GassimModel;

void GassimNode::next() {					// proceed to the next step...
	_out = _nextOut;
}


void GassimNode::connect(NodePtr_t &nodePtr) {
	// TODO do somehting better than assertions.
	unsigned otherNodeId = nodePtr->nodeId();
	assert(nodeId() != otherNodeId);
	assert(_connections.find(otherNodeId) == _connections.end());
	_connections[otherNodeId] = nodePtr;
}


GassimNode::NodePtr_t GassimNode::getOtherConnection(NodePtr_t &nodePtr) {
	// todo, do something better than assertsions herelll
	assert(_connections.size() == 2);
	for (auto n : _connections) {
		if (n.second->nodeId() != _nodeId) 
			return (n.second);
	}
	return(NULL);
}
