#ifndef __GASSIM_NODE_H__
#define __GASSIM_NODE_H__

#include <memory>
#include <string>
#include <vector>
#include <set>


#include "GassimVars.h"

class GassimNode
{
public:
	typedef enum
	{
		MODEL =0,
	    GASSOURCE=1,
	    LUNGS=2,
	    PIPE=3,
	    RESEROVIRE=4
	} NodeType;
	GassimNode(const std::string &name, NodeType nodeType) :
		_name(name),
		_nodeType(nodeType),
		_nodeId(_nextId++),
		_steps(0) 	{};
	GassimNode() = delete;
	virtual ~GassimNode(){}
	
	GassimVars out;    // public, but constant reference...
	
	
	unsigned nodeId() const { return (_nodeId); }
	NodeType nodeType() const {return (_nodeType);}
	virtual void step(double dt);
	virtual void next();
	std::string name() const	{return (_name);	}	;
	unsigned getNumConnections() const	{ return (_connections.size());	}
	virtual std ::string getValues() const ;   // this might map better with osstr
	virtual std ::string getHeader() const;
	bool addChildNode(std::shared_ptr<GassimNode> nodePtr);
	bool connect(std::shared_ptr<GassimNode> nodePtr, bool connBack=true);
	void setPressure(double pressure) { _nextOut.pressure = pressure; 	}
	void setTemp(double temp) 	{ _nextOut.temp = temp; }
	void setVolume(double volume) { _nextOut.volume = volume; 	}
	void setResistance(double resistance) 	{ _nextOut.resistance = resistance; }
	void setPO2(double pO2)	{ _nextOut.pO2 = pO2; 	}
	void setOpen(double open) { _nextOut.open = open; 	}
	void setAllTemp(double temp);
	virtual double getPressureDrop()	{ return (0); }
protected:
	virtual  void init();
private:
	static unsigned _nextId;
	unsigned _nodeId;
	std::string _name;
	NodeType _nodeType;
	GassimVars _nextOut;        // next output state
	std::vector<std::shared_ptr<GassimNode>> _childNodes;
	std::vector<std::shared_ptr<GassimNode>> _connections;  // graph connections.
	std::set<unsigned> _childSet;           // make sure we don't already have the node in our set...
	uint64_t _steps;
	
};
#endif
