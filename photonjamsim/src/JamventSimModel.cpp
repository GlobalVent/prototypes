

#include "JamventSimModel.h"




JamventSimModel::JamventSimModel() :
	_dt(0),
	_log(nullptr)
{
	double o2SrcPres  = 4.0;
	double airSrcPres = 4.0;
	double valveABrst = 4.5;		
	double valveCrst  = 1.11;
	double valveDrst  = 0.029;

    // these are nomial default values, change the parameters prior to starting
	//    based on whatever we want our test configuration input to be...
	_o2Src = std ::make_shared<GassimSource>("o2Src",   o2SrcPres,  1.00);	  // 3 bar guage, 4 absoluate...
	_airSrc = std ::make_shared<GassimSource>("airSrc", airSrcPres, 0.21);	  // 3 bar guage, 4 absoluate...
	_valveA = std ::make_shared<GassimPipe>("valveA", valveABrst, false);
	_valveB = std ::make_shared<GassimPipe>("valveB", valveABrst, false);
	_valveC = std ::make_shared<GassimPipe>("valveC", valveCrst, false);
	_valveD = std ::make_shared<GassimPipe>("valveD", valveDrst, false);
	_reservoir = std ::make_shared<GassimReservoir>("reservoir", 1.0, 2.0);  // 1 bar at 2 liter bottle
	
	// 2.5 liter lungs, compliance of .035 cmh20
	_lungs = std ::make_shared<GassimLungs>("reservoir", 1.0, 2.5, GassimConv::complianceCmH2oToBar(.035));  
	// _lungs = std ::make_shared<GassimLungs>("reservoir", 1.0, 2.5, 20);  
	//_lungs = std ::make_shared<GassimReservoir>("lungs", 1, 100);  
	_gasSink = std ::make_shared<GassimSource>("gasSink", 1.0, 0.21);	  // gas sink at 1 bar...

	_model.addNode(_o2Src);
	_model.addNode(_airSrc);
	_model.addNode(_valveA);
	_model.addNode(_valveB);
	_model.addNode(_valveC);
	_model.addNode(_valveD);
	_model.addNode(_reservoir);
	_model.addNode(_lungs);
	_model.addNode(_gasSink);

	// and now connect them.
	_model.connect(_o2Src,_valveA);
	_model.connect(_valveA,_reservoir);
	_model.connect(_airSrc,_valveB);
	_model.connect(_valveB,_reservoir);
	_model.connect(_reservoir,_valveC);
	_model.connect(_valveC,_lungs);
	_model.connect(_lungs,_valveD);
	_model.connect(_valveD,_gasSink);

}
JamventSimModel::~JamventSimModel()
{
}


/**
 * @brief setDebugLevel
 * @details set the debug level to use for debugging 
 *          must have done the setLogStream below
 * 
 * @param level  -- debug level to set, 0, no debug data. 1,2,3... increasing verbosity
 */
void JamventSimModel::setDebugLevel(unsigned level) {
	// TBD.. do something with this.
}

/**
 * @brief init
 *    initialize timestep 0, needed if you want to plot the initialial sate
 */
void JamventSimModel::init() {
	_model.init();
}

/**
 * @brief step -- step the simulation one delta time.
 * @details step the simulaton one time step.
 * 
 * @param dt -- delta time since last time step.
 */
void JamventSimModel::step(float dt) {
	_model.next();
	_model.step(dt);
	_dt+=dt;
}


/**
 * @brief set the valve state for the simulator
 * @details manullly override the valve state for a given valve.
 * 
 * @param open [description]
 */
void JamventSimModel::setValveAopen(bool open) {
	_valveA->setOpen(open);
}
void JamventSimModel::setValveBopen(bool open) {
	_valveB->setOpen(open);
}
void JamventSimModel::setValveCopen(bool open) {
	_valveC->setOpen(open);
}
void JamventSimModel::setValveDopen(bool open) {
	_valveD->setOpen(open);
}

/**
 * @brief retrieve the current valve state.
 * @details read back the current valve state.
 * @return true -- open, false -- closed
 */
bool JamventSimModel::getValveAopen() {
	return(_valveA->open());
}
bool JamventSimModel::getValveBopen() {
	return(_valveB->open());
}
bool JamventSimModel::getValveCopen() {
	return(_valveC->open());
}
bool JamventSimModel::getValveDopen() {
	return(_valveD->open());
}

/**
 * @brief getPsys -- get the psys pressure.
 * @details retrieve the Psys pressure.
 * @return Psys pressure in bars... (for now)...
 */
float JamventSimModel::getPsys() {
	return(_lungs->pressure());
}

/**
 * @brief retrieve the Pref pressure
 * @details retroeve the Pres -- reservoire pressure.
 * @return Pres pressure in bars
 */
float JamventSimModel::getPres() {
	return(_reservoir->pressure());
}

/**
 * @brief retrieve the Po2 level
 * @details read back the p02 level 0.00 - 1.0....
 * @return [description]
 */
float JamventSimModel::getPO2() {
	return(_lungs->pO2());
}

/**
 * @brief set the o2 source pressure in bar
 * @details set the o2 source pressure.
 *          at any point in the simulation to change the source pressure.
 * 
 * @param pressure [description]
 */
void JamventSimModel::setO2Pressure(double pressure) {
	_o2Src->setPressure(pressure);
}

/**
 * @brief set the Air source pressure in bars
 * @details set the air source pressure
 *          at any point in the simulation to change the source pressure.
 * 
 * @param pressure [description]
 */
void JamventSimModel::setAirPressure(double pressure) {
	_airSrc->setPressure(pressure);
}

/**
 * @brief set the lung compliance
 *     use Conversion routines to convert to and from cmH20
 * @param compliance  in  (delta)liters
 * 						 ---------------
 * 						  (delta)bar
 */
void JamventSimModel::setLungCompliance(double compliance) {
	_lungs->setCompliance(compliance);
}


/**
 * @brief Get the lung volume value.
 * 
 * @return lung volume
 */
float JamventSimModel::getLvol() {
	return(_lungs->volume());
}

