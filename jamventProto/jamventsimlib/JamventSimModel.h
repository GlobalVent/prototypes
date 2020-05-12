#ifndef __JAMVENT_SIM_MODEL_H__
#define __JAMVENT_SIM_MODEL_H__

#include <memory>
#include <string>
#include "GassimModel.h"


/**
 * @brief Jamvent simulation model
 * @details Jamvent simuluation model
 *          implements an underlying gassim model for
 *          jamvent, it is entirely state driven and is expected to be
 *          called at approximitly 10ms intervals.
 * 
 * @param d0 [description]
 * @param s [description]
 * 
 * @return [description]
 */
class JamventSimModel {
public:
	JamventSimModel();
	;
	virtual ~JamventSimModel();



	/**
	 * @brief setDebugLevel
	 * @details set the debug level to use for debugging 
	 *          must have done the setLogStream below
	 * 
	 * @param level  -- debug level to set, 0, no debug data. 1,2,3... increasing verbosity
	 */
	virtual void setDebugLevel(unsigned level);
	/**
	 * @brief setLogStream
	 * @details set the string stream to use for logging debug information.
	 * 
	 * @param ostr [description]
	 */
	virtual void setLogStream(std::ostream &ostr);

	/**
	 * @brief step -- step the simulation one delta time.
	 * @details step the simulaton one time step.
	 * 
	 * @param dt -- delta time since last time step.
	 */
	virtual void step(float dt);

	/**
	 * @brief getCurrDt -- get the current delta time
	 * @details retrieve the current delta time since we started
	 *          this simulation
	 * @return delta time since initial condition.
	 */
	virtual float getCurrDt() {return (_dt);}
	;

	/**
	 * @brief set the valve state for the simulator
	 * @details manullly override the valve state for a given valve.
	 * 
	 * @param open [description]
	 */
	virtual void setValveAopen(bool open);
	virtual void setValveBopen(bool open);
	virtual void setValveCopen(bool open);
	virtual void setValveDopen(bool open);

	/**
	 * @brief retrieve the current valve state.
	 * @details read back the current valve state.
	 * @return true -- open, false -- closed
	 */
	virtual bool getValveAopen();
	virtual bool getValveBopen();
	virtual bool getValveCopen();
	virtual bool getValveDopen();

	/**
	 * @brief getPsys -- get the psys pressure.
	 * @details retrieve the Psys pressure.
	 * @return Psys pressure in bars... (for now)...
	 */
	virtual float getPsys();

	/**
	 * @brief retrieve the Pref pressure
	 * @details retroeve the Pres -- reservoire pressure.
	 * @return Pres pressure in bars
	 */
	virtual float getPres();		

	/**
	 * @brief retrieve the Po2 level
	 * @details read back the p02 level 0.00 - 1.0....
	 * @return [description]
	 */
	virtual float getPo2();

	/**
	 * @brief set the o2 source pressure in bar
	 * @details set the o2 source pressure.
	 *          at any point in the simulation to change the source pressure.
	 * 
	 * @param pressure [description]
	 */
	virtual void  setO2Pressure(double pressure);

	/**
	 * @brief set the Air source pressure in bars
	 * @details set the air source pressure
	 *          at any point in the simulation to change the source pressure.
	 * 
	 * @param pressure [description]
	 */
	virtual void setAirPressure(double pressure);

	/**
	 * @brief set the lung compliance
	 * 
	 * @param compliance  in  (delta)liters
	 * 						 ---------------
	 * 						  (delta)bar
	 */
	virtual void setLungCompliance(double compliance);


protected:
	GassimModel	_model;

	GassimNode::NodePtr_t _o2Src;
	GassimNode::NodePtr_t _airSrc;
	GassimNode::NodePtr_t _valveA;
	GassimNode::NodePtr_t _valveB;
	GassimNode::NodePtr_t _valveC;
	GassimNode::NodePtr_t _valveD;
	GassimNode::NodePtr_t _reservoir;
	GassimNode::NodePtr_t _lungs;
	GassimNode::NodePtr_t _gasSink;

	float _dt;



private:

};


#endif

