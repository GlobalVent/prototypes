

#include "JamventSimModel.h"


JamventSimModel::JamventSimModel() :
	_dt(0)
{
	double pipeResist=0.1;

	_o2Src = std ::make_shared<GassimSource>("o2Src", 5.0, 1.00);	  // 4 bar guage, 5 absoluate...
	_airSrc = std ::make_shared<GassimSource>("airSrc", 5.0, 0.21);	  // 4 bar guage, 5 absoluate...
	_valveA = std ::make_shared<GassimPipe>("valveA", pipeResist, false);
	_valveB = std ::make_shared<GassimPipe>("valveB", pipeResist, false);
	_valveC = std ::make_shared<GassimPipe>("valveC", pipeResist, false);
	_valveD = std ::make_shared<GassimPipe>("valveD", pipeResist, false);
	_reservoir = std ::make_shared<GassimReservoir>("reservoir", 1.0, 2.0);  // 1 bar at 2 liter bottle
	_lungs = std ::make_shared<GassimReservoir>("reservoir", 1.0, 4.0);  // 4 liter lungs
	_gasSink = std ::make_shared<GassimSource>("gasSink", 1.0, 1.00);	  // gas sink at 1 bar...


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
 * @brief setLogStream
 * @details set the string stream to use for logging debug information.
 * 
 * @param ostr [description]
 */
void JamventSimModel::setLogStream(std::ostream &ostr) {
	// TBD do something with this.
}

/**
 * @brief step -- step the simulation one delta time.
 * @details step the simulaton one time step.
 * 
 * @param dt -- delta time since last time step.
 */
void JamventSimModel::step(float dt) {
	_dt+=dt;
	_model.step(dt);
	_model.next();
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
	return(_valveC->open());
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
float JamventSimModel::getPo2() {
	return(_reservoir->pO2());
}


