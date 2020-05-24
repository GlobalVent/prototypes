#ifndef __JAM_CTRL_SIM_H__
#define __JAM_CTRL_SIM_H__

#include "JamCtrlMgr.h"
#include "JamventSimModelRt.h"

class JamCtrlSim : public JamCtrlMgr
{
public:
    JamCtrlSim();
    virtual ~JamCtrlSim();

	virtual void init();

	/**
	 * @brief manually set the valve state
	 * @details manullly override the valve state for a given valve.
	 * 
	 * @param open true open the valve, false, close it.
	 */
	virtual void setValveAopen(bool open) {_jamModel.setValveAopen(open);}
	virtual void setValveBopen(bool open) {_jamModel.setValveBopen(open);}
	virtual void setValveCopen(bool open) {_jamModel.setValveCopen(open);}
	virtual void setValveDopen(bool open) {_jamModel.setValveDopen(open);}



protected:
	/**
	 * @brief step the model one time step.
	 *        there is one implementation for the 
	 *        simululator and one for real hardware layer...
	 * @param dt delta time since last time we were called.
	 */
	virtual void step(double dt);		// step the model

private:
    JamventSimModelRt _jamModel;

};

#endif
