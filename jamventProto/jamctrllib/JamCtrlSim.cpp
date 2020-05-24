

#include "JamCtrlSim.h"
#include "fmtstr.h"

using namespace std;



JamCtrlSim::JamCtrlSim()
{

}
JamCtrlSim::~JamCtrlSim()
{
    
}

void JamCtrlSim::init() {
    JamCtrlMgr::init();
}

/**
 * @brief step the model one time step.
 *        there is one implementation for the 
 *        simululator and one for real hardware layer...
 * @param dt delta time since last time we were called.
 */
void JamCtrlSim::step(double dt) {
    JamCtrlMgr::step(dt);       // give our the parent state processor a shot...
    _jamModel.step(dt);         // then do our step...

    // read back our basic data from the simulator here...
	_ctrlData.pRes = _jamModel.getPres();
    _ctrlData.pSys = _jamModel.getPsys();
    _ctrlData.pO2  = _jamModel.getPO2();

    _ctrlData.valveAopen = _jamModel.getValveAopen();
    _ctrlData.valveBopen = _jamModel.getValveBopen();
    _ctrlData.valveCopen = _jamModel.getValveCopen();
    _ctrlData.valveDopen = _jamModel.getValveDopen();

}
