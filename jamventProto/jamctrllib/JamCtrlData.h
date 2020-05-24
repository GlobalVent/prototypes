#ifndef __JAM_CTRL_DATA_H__
#define __JAM_CTRL_DATA_H__

/**
 * @brief JamventCtrlData
 * @details Container class returning the 
 *          ccontrol system telemetry.
 * 
 */
class JamCtrlData {
public:
	JamCtrlData() :
		fiO2_set(0),
		tidalVolume_set(0),
		respRate_set(0),
		peepIeRatio_set(0),
		pRes(0),
		pSys(0),
		pO2(0),
        lvol(0),
		valveAopen(0),		// valve state
		valveBopen(0),
		valveCopen(0),
		valveDopen(0)
	{};
	//  These first values are readbacks of the setXXX functions in jamventCtrlMgr;
	double fiO2_set;
	double tidalVolume_set;
	double respRate_set;
	double peepIeRatio_set;


	// TODO: add readback of calibration data here...

	// active values (note no _set suffix.
	double pRes;				// resovior pressure.
	double pSys;				// psys pressure
	double pO2;				// po2 sensor reading.

    // temporary placeholder for lung volume.. for initial test.
    double lvol;             // lung volume

	bool  valveAopen;		// valve state
	bool  valveBopen;
	bool  valveCopen;
	bool  valveDopen;

protected:
private:
};

#endif
