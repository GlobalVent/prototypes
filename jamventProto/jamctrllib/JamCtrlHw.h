#ifndef __JAM_CTRL_HW_H__
#define __JAM_CTRL_HW_H__

#include "JamCtrlMgr.h"

class JamCtrlHw : public JamCtrlMgr
{
public:
	JamCtrlHw();
	virtual ~JamCtrlHw();

	virtual void init();

protected:
	/**
	 * @brief step the model one time step.
	 *        there is one implementation for the 
	 *        simululator and one for real hardware layer...
	 * @param dt delta time since last time we were called.
	 */
	virtual void step(double dt);		// step the model

private :


};

#endif
