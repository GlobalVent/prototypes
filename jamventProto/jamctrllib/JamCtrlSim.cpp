

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
    
	//_ctrlData.pRes_cmh2o = GassimConv::barToCmH2o(_jamModel.getPres()-1);
    _ctrlData.pRes_bar = _jamModel.getPres() - 1;
    //_ctrlData.pSys_cmh2o = GassimConv::barToCmH2o(_jamModel.getPsys()-1);
    _ctrlData.pSys_bar = _jamModel.getPsys() - 1;
    _ctrlData.o2_pc  = _jamModel.getPO2();
    _ctrlData.lungVol_ml = _jamModel.getLvol();

    _ctrlData.isAOpen = _jamModel.getValveAopen();
    _ctrlData.isBOpen = _jamModel.getValveBopen();
    _ctrlData.isCOpen = _jamModel.getValveCopen();
    _ctrlData.isDOpen = _jamModel.getValveDopen();
}
