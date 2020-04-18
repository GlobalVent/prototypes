
#include "GassimNode.h"

using namespace std;

unsigned GassimNode::_nextId = 0;


void GassimNode::step(double dt)
{
	if (_steps == 0)	
		init();
	
	for (auto n : _childNodes)
		n->step(dt);
}
void GassimNode::next()
{
	_steps++;	
	for (auto n : _childNodes)
		n->next();
	//GassimVars &rout = const_cast<GassimVars>(&out);	
	out = _nextOut;
}
void GassimNode::init()
{
	for (auto n : _childNodes)
		n->init();
}
string GassimNode::getValues() const
{
	return (string(""));
}
string GassimNode::getHeader() const
{
	return (string(""));
}
bool GassimNode::addChildNode(std::shared_ptr<GassimNode> nodePtr)
{
	if (nodePtr->nodeId() == _nodeId)
		return (false);
	_childNodes.push_back(nodePtr);
	_childSet.insert(nodePtr->nodeId());
	return (true);
}
bool GassimNode::connect(std::shared_ptr<GassimNode> nodePtr, bool connBack)
{
	if (nodePtr->nodeId() == _nodeId)
		return (false);
	_connections.push_back(nodePtr);
	return (true);
}
void GassimNode::setAllTemp(double temp)
{
	setTemp(temp);
	for (auto c : _childNodes) 
		c->setAllTemp(temp);
}

