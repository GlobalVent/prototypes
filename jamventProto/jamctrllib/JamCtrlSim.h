#ifndef __JAM_CTRL_SIM_H__
#define __JAM_CTRL_SIM_H__

#include "JamCtrlMgr.h"
#include "JamventSimModelRt.h"

class JamCtrlSim : public JamCtrlMgr
{
public:
    JamCtrlSim();
    virtual ~JamCtrlSim();

	virtual void init() override;

	/**
	 * @brief manually set the valve state
	 * @details manullly override the valve state for a given valve.
	 * 
	 * @param open true open the valve, false, close it.
	 */
	void setValveAopen(bool open) override {_jamModel.setValveAopen(open);}
	void setValveBopen(bool open) override {_jamModel.setValveBopen(open);}
	void setValveCopen(bool open) override {_jamModel.setValveCopen(open);}
	void setValveDopen(bool open) override {_jamModel.setValveDopen(open);}

    void setFio2(IntegerNumType fiO2_pc) override {};
    void setTidalVol(IntegerNumType tidalVol_ml) override {};
    void setRespRate(IntegerNumType respRate) override {};
    void setIeRatio(IntegerNumType ieRatio) override {};
    void setPeep(FloatNumType peep_bar) override {};

  protected:
	/**
	 * @brief step the model one time step.
	 *        there is one implementation for the 
	 *        simululator and one for real hardware layer...
	 * @param dt delta time since last time we were called.
	 */
	virtual void step(double dt) override;  // step the model

private:
    JamventSimModelRt _jamModel;

};

#endif
