#ifndef __JAMVENT_SIM_MODEL_RT_H__
#define __JAMVENT_SIM_MODEL_RT_H__

#include "JamventSimModel.h"
#include <set>

/**
 * @brief JamventSimModelRt Jamvent Sim Model Real time.
 * 
 * Model with delays on inputs and outputs to simulate the delays for
 *   * valve delay
 *   * sensor measurement lag time.
 * 
 * the model is stepped just like JamventSimModel, calls are to the 
 *    measurements are intercepted and subject to delays
 *    There is also a Raw form of each of the functions
 *    so that one can at the measurement without any delays...
 */

class JamventSimModelRt : public JamventSimModel {
public:
	JamventSimModelRt();
	virtual ~JamventSimModelRt();

	/**
	 * @brief step -- step the simulation one delta time.
	 * @details step the simulaton one time step.
	 * 
	 * @param dt -- delta time since last time step.
	 */
	virtual void step(float dt);

	/**
	 * @brief set the valve state for the simulator
	 * @details manullly override the valve state for a given valve.
	 * 
	 * @param open true to open the valve, false to close..
	 */
	virtual void setValveAopen(bool open);
	virtual void setValveBopen(bool open);
	virtual void setValveCopen(bool open);
	virtual void setValveDopen(bool open);

	virtual void setValveAopenRaw(bool open) { JamventSimModel::setValveAopen(open);}
	virtual void setValveBopenRaw(bool open) { JamventSimModel::setValveBopen(open);}
	virtual void setValveCopenRaw(bool open) { JamventSimModel::setValveCopen(open);}
	virtual void setValveDopenRaw(bool open) { JamventSimModel::setValveDopen(open);}



	/**
	 * @brief Set the Valve delay
	 * 
	 * @param dly -- in seconds, so for 10 ms delay use 0.010
	 */
	virtual void setValveDly(float dly) {_valveDly=dly;	}

	/**
	 * @brief Set the pressure delay 
	 *        set delays for Pres and Psys
	 * 
	 * @param dly -- in seconds, so for 10 ms delay use 0.010
	 */
	virtual void setPressDly(float dly) {_pressDly = dly;}

	/**
	 * @brief set the o2 sensor delay
	 * 
	 * @param dly -- in seconds, so for 10 ms delay use 0.010
	 */
	virtual void setO2Dly(float dly) {_pO2Dly = dly;}

	/**
	 * @brief getPsys -- get the psys pressure.
	 * @details retrieve the Psys pressure.
	 * @return Psys pressure in bars... (for now)...  
	 *         use GassimConv to convert to bars
	 */
	virtual float getPsys();
	virtual float getPsysRaw() {return(JamventSimModel::getPsys());}

	/**
	 * @brief retrieve the Pref pressure
	 * @details retroeve the Pres -- reservoire pressure.
	 * @return Pres pressure in bars
	 */
	virtual float getPres();		
	virtual float getPresRaw() {return(JamventSimModel::getPres());}

	/**
	 * @brief retrieve the Po2 level
	 * @details read back the p02 level 0.00 - 1.0....
	 * @return percent O2
	 */
	virtual float getPO2();
	virtual float getPO2Raw() {return(JamventSimModel::getPO2());}

	/**
	 * @brief Get the lung volume value.
	 * 
	 * @return lung volume
	 */
	virtual float getLvol();
	virtual float getLvolRaw() {return(JamventSimModel::getLvol());}

protected:
	// Structures to keep track of the delays by time.
	typedef std::map<float,bool> DelayValveMap_t;
	typedef std::map<float,double> DelaySensorMap_t;
	void processValveDly();
	void processSensorDly();
	bool getValveWithDly(DelayValveMap_t &dlyMap, bool &newValue);
	bool getSensorWithDly(DelaySensorMap_t &dlyMap, float &newVal);

	float _valveDly;

	float _pressDly;
	float _pO2Dly;

	float _pSys;			// our current state values
	float _pRes;			// for the sensors..
	float _pO2;
	float _lvol;			// lung volume delayed...

	DelayValveMap_t _valveAdlyMap;
	DelayValveMap_t _valveBdlyMap;
	DelayValveMap_t _valveCdlyMap;
	DelayValveMap_t _valveDdlyMap;

	DelaySensorMap_t _PresDlyMap;
	DelaySensorMap_t _PsysDlyMap;
	DelaySensorMap_t _PO2DlyMap;
	DelaySensorMap_t _LvolDlyMap;
	

private:

};

#endif
