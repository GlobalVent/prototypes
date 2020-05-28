
#include <vector>

#include "JamventSimModelRt.h"
//#include "JamventTime.h"

using namespace std;

JamventSimModelRt::JamventSimModelRt() :
	JamventSimModel(),
	_valveDly(0.01),
	_pressDly(0.01),
	_pO2Dly(0.01),
	_pSys(1.0),
	_pRes(1.0),
	_pO2(.21)

{
	// empty constructor
}

JamventSimModelRt::~JamventSimModelRt()
{
	// empty destructor.
}

/**
 * @brief get a valve value from the delay map.
 * 
 * @param dlyMap -- delay map to get the values from.
 * @param newValue -- new value to use
 * @return true -- true if there IS a new value.
 * @return false  -- false if there is no change...
 */
bool JamventSimModelRt::getValveWithDly(JamventSimModelRt::DelayValveMap_t &dlyMap, 
                                        bool &newVal) {
	vector<float> removeList;
	bool found = false;
	for (auto const& vdly : dlyMap) {
		if (vdly.first > _dt)
			break;
		removeList.push_back(vdly.first);		// remember to remove these.
		found = true;
		newVal = vdly.second;
	}
	for (auto const &it : removeList) // easier to do this later than doing it in place.
		dlyMap.erase(it);

	return(found);

}
bool JamventSimModelRt::getSensorWithDly(JamventSimModelRt::DelaySensorMap_t &dlyMap, 
                                           float &newVal) {
	vector<float> removeList;
	bool found = false;
	for (auto const& sdly : dlyMap) {
		if (sdly.first > _dt)
			break;
		removeList.push_back(sdly.first);		// remember to remove these.
		found = true;
		newVal = sdly.second;
	}
	for (auto const &it : removeList) // easier to do this later than doing it in place.
		dlyMap.erase(it);
	return(found);
}

/**
 * @brief process the valve delays
 * 
 */
void JamventSimModelRt::processValveDly() {
	bool open;
	if (getValveWithDly(_valveAdlyMap, open)) 
		setValveAopenRaw(open);
	if (getValveWithDly(_valveBdlyMap, open)) 
		setValveBopenRaw(open);
	if (getValveWithDly(_valveCdlyMap, open)) 
		setValveCopenRaw(open);
	if (getValveWithDly(_valveDdlyMap, open)) 
		setValveDopenRaw(open);
}
/**
 * @brief processor the sensor delays
 * 
 */
void JamventSimModelRt::processSensorDly() {

	// float _pressDly;
	// float _pO2Dly;

	if (_pressDly > 0) {
		getSensorWithDly(_PsysDlyMap, _pSys);
		getSensorWithDly(_PresDlyMap, _pRes);
		getSensorWithDly(_LvolDlyMap, _lvol);
		_PsysDlyMap[_dt+_pressDly] = getPsysRaw();
		_PresDlyMap[_dt+_pressDly] = getPresRaw();
		_LvolDlyMap[_dt+_pressDly] = getLvolRaw();
	}
	else {
		_pSys = getPsysRaw();
		_pRes = getPresRaw();
		_lvol = getLvolRaw();
	}
	if (_pO2Dly > 0) {
		getSensorWithDly(_PO2DlyMap, _pO2);
		_PO2DlyMap[_dt+_pO2Dly] = getPO2Raw();
	}
	else {
		_pO2 = getPO2Raw();
	}
}
/**
 * @brief step -- step the simulation one delta time.
 * @details step the simulaton one time step.
 * 
 * @param dt -- delta time since last time step.
 */
void JamventSimModelRt::step(float dt) {
	processValveDly();			// process all valve settings.
	processSensorDly();			// process all sensor delays
	JamventSimModel::step(dt);		// do the parent step
}

/**
 * @brief set the valve state for the simulator
 * @details manullly override the valve state for a given valve.
 * 
 * @param open [description]
 */
void JamventSimModelRt::setValveAopen(bool open) {
	if (_valveDly == 0) 
		JamventSimModel::setValveAopen(open);
	else 
		_valveAdlyMap[_dt+_valveDly] = open;
}
void JamventSimModelRt::setValveBopen(bool open) {
	if (_valveDly == 0) 
		JamventSimModel::setValveBopen(open);
	else 
		_valveBdlyMap[_dt+_valveDly] = open;
}
void JamventSimModelRt::setValveCopen(bool open) {
	if (_valveDly == 0) 
		JamventSimModel::setValveCopen(open);
	else 
		_valveCdlyMap[_dt+_valveDly] = open;

}
void JamventSimModelRt::setValveDopen(bool open) {
	if (_valveDly == 0) 
		JamventSimModel::setValveDopen(open);
	else 
		_valveDdlyMap[_dt+_valveDly] = open;
}


/**
 * @brief getPsys -- get the psys pressure.
 * @details retrieve the Psys pressure.
 * @return Psys pressure in bars... (for now)...  
 *         use GassimConv to convert to bars
 */
float JamventSimModelRt::getPsys() {
	return(_pSys);
}

/**
 * @brief retrieve the Pref pressure
 * @details retroeve the Pres -- reservoire pressure.
 * @return Pres pressure in bars
 */
float JamventSimModelRt::getPres() {
	return(_pRes);
}

/**
 * @brief retrieve the Po2 level
 * @details read back the p02 level 0.00 - 1.0....
 * @return percent O2
 */
float JamventSimModelRt::getPO2() {
	return(_pO2);
}

/**
 * @brief Get the lung volume value.
 * 
 * @return lung volume
 */
float JamventSimModelRt::getLvol()  {
	return(_lvol);
}

