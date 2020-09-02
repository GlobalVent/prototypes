/*
 * FILE: JamCtrlGasMix.h
 *
 * LICENSE: This file is licensed under the GNU General Public License v3.0
 * Permissions of this strong copyleft license are conditioned on making available complete 
 * source code of licensed works and modifications, which include larger works using a licensed 
 * work, under the same license. Copyright and license notices must be preserved. Contributors 
 * provide an express grant of patent rights.
 * 
 * DESCRIPTION: This class executes the math to calculate the pressures necessary to achieve
 * the desired gas mix (pressure & percent oxygen).
 * The math is derived from "The Mathematics Behind	Partial	Pressure Gas Blending" v1.0 by Gary C. Kessler
 **/

#ifndef __JAM_CTRL_GAS_MIX_H__
#define __JAM_CTRL_GAS_MIX_H__

class JamCtrlGasMix 
{
public:
	JamCtrlGasMix() {};
	virtual ~JamCtrlGasMix() {}
	
	/* initializes/calibrates gas mix variables */
	void initGasMix();
	/* calculates the necessary pressures for fill (oxygen) and top-up (air) gasses 
	 * to achieved the desired mix & pressure */
	void calcGasMix(const float * const pressureTotalDesired_bar,
					const float * const oxyPercDesired_fracDec,
					const float * const pressureStarting_bar,
					const float * const currOxyPerc_fracDec,
					float * const reqOxyFillPressure_bar,
					float * const reqAirTopUpPressure_bar);


protected:	
	
private:

	/* The fixed fraction of oxygen in the fill gas as a decimal */
	float mixOxygenSource_fracDec;
	/* The fixed fraction of oxygen in the top off gas as a decimal */
	float mixAirSource_fracDec;
	

	/* calculate fill pressure for oxygen */
	void calcPresOxygenFill(const float * const pressureTotalDesired_bar,
		const float * const oxyPercDesired_fracDec,
		const float * const pressureStarting_bar,
		const float * const oxyPercStarting_fracDec,
		float * const reqOxyFillPressure_bar);
	/* calculate top off pressure for air */
	void calcPresAirTopOff(const float * const pressureTotalDesired_bar,
		const float * const pressureStarting_bar,
		float * const reqOxyFillPressure_bar,
		float * const reqAirTopUpPressure_bar);
	
	/* @ param startingTerm: Ps * (Ms - Mt) 
	 * starting pressure * (starting mix - frac oxy in top off) */
	float startingTerm;
	/* @param desiredTerm: Pd * (Ms - Mt)
	 * desired total pressure * (desired frac oxy - frac oxy in top off) */
	float desiredTerm;
	/* @param fillTerm: (Mf - Mt)
	 * oxy frac of fill gas - oxy fraction of top off gas */
	float fillTerm;
	
};
#endif
