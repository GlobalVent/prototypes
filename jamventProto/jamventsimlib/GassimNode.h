#ifndef __GASSIM_NODE_H__
#define __GASSIM_NODE_H__

#include <string>
#include <vector>
#include <set>
#include <memory>
#include <map>
#include <ostream>

#include "GassimVars.h"
#include "JamsimDbgPrint.h"


/**
 * @brief GassimNode -- simulation node...
 * @details GassimNode -- network simulation nodes
 * 
 * 
 */
class GassimNode {
public:
	typedef enum {
		GASSOURCE=1,
		LUNGS=2,
		PIPE=3,
		RESERVOIR=4
	} NodeType;
	typedef std::shared_ptr<GassimNode> NodePtr_t;		// prefered method for moving pointers of thes things around.
	typedef std::map<unsigned, NodePtr_t> ConnectionMap_t;
	GassimNode() = delete;			// no default constructor..
	GassimNode(const std::string &name, NodeType nodeType);
	virtual ~GassimNode() {};
	/**
	 * @brief Set the Log Stream object (for debug telemetry)
	 * 
	 * @param ostr 
	 */
	virtual void setLog(JamsimDbgPrint *log) {
		_log = log;
	}


	// set and get for the internal vars..., 
	//   get the current value.
	//   set the next.
	//     GassimNode
	virtual double pressure() { return(_out.pressure);};
	virtual void   setPressure(double pressure) { _nextOut.pressure = pressure;};

	virtual double volume() {return(_out.volume);};
	virtual void   setVolume(double volume) {_nextOut.volume = volume;};

	virtual double temp() {return(_out.temp);};
	virtual void   setTemp(double temp) {_nextOut.temp = temp;};

	virtual double pO2() {return(_out.pO2);};
	virtual void setPO2(double pO2) {_nextOut.pO2 = pO2;};

	virtual double flow() {return(_out.flow);};
	virtual void   setFlow(double flow) {_nextOut.flow = flow;};

	virtual double resistance() {return (_out.resistance);};
	virtual void   setResistance(double resistance) {_nextOut.resistance=resistance;};

	virtual double compliance() {return(_out.compliance);};
	virtual void   setCompliance(double compliance) {_nextOut.compliance=compliance;};

	virtual bool   open() {return(_out.open);};
	virtual void   setOpen(bool open) {_nextOut.open = open;};

	virtual double getPressureDrop(unsigned nodeId) {return(0);};		// for pipes override this, 

	unsigned nodeId() { return(_nodeId);};
	void     setNodeId(unsigned nodeId) {_nodeId = nodeId;};  // called only by the SimModel on addNode...

	virtual void step(double dt) = 0;		// step the simulation for delta time.
	virtual void next();					// proceed to the next step...

	std::string name() {return (_name);};


	virtual unsigned getNumConnections() { return(_connections.size());};
	virtual NodePtr_t getOtherConnection(NodePtr_t &nodePtr);

	virtual void connect(NodePtr_t &nodePtr);



protected:
	JamsimDbgPrint *_log;
	std::string _name;
	unsigned _stepCount;
	unsigned  _nodeId;
	NodeType _nodeType;
	ConnectionMap_t _connections;			// node connections.

	GassimVars _out;
	GassimVars _nextOut;

private:

};

#endif

