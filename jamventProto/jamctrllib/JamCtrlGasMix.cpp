/*
 * FILE: JamCtrlGasMix.cpp
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

#include "JamCtrlGasMix.h"


/**
 * @brief Initialize (and TODO: calibrate?) gas mix class variables 
 */
void JamCtrlGasMix::initGasMix() 
{
	/* TODO: what other initialization is needed? */
	this->startingTerm = 0.0;
	this->desiredTerm = 0.0;
	this->fillTerm = 0.0;
	
	/* TODO: calibrate mixOxygenSource_fracDec? */
	this->mixOxygenSource_fracDec = 1.0;
	/* TODO: calibrate: mixAirSource_fracDec? */
	this->mixAirSource_fracDec = 0.209;
}

/**
 * @brief calculates the necessary pressures for fill (oxygen) and top-up (air) gasses 
 *			to achieved the desired mix & pressure 
 * @param pressureTotalDesired_bar: The target pressure for the reservoir gas mix
 * @param oxyPercDesired_fracDec: Desired oxygen percentage as a fractional decimal. (100% = 1.0)
 * @param pressureStarting_bar: The pressure within the resevoir at the end of the 
 *			inhalation phase
 * @param oxyPercStarting_fracDec: The oxygen percentage within the reservoir at the 
 *			end of the inhalation phase as a fractional decimal
 * @param reqOxyFillPressure_bar: The required pressure of oxygen to add to the 
 *			reservoir to achieve desired mix in bar
 * @param reqAirTopUpPressure_bar: The required pressure of air to add to the 
 *			reservoir to achieve desired mix in bar.
 */
void JamCtrlGasMix::calcGasMix( const float * const pressureTotalDesired_bar,
								const float * const oxyPercDesired_fracDec,
								const float * const pressureStarting_bar,
								const float * const oxyPercStarting_fracDec,
								float * const reqOxyFillPressure_bar,
								float * const reqAirTopUpPressure_bar)
{
	/* step 0: range check inputs */
	/* TODO: range check inputs */
	
	/* step 1: calculate fill pressure for oxygen  */
	calcPresOxygenFill(pressureTotalDesired_bar,
							oxyPercDesired_fracDec, 
							pressureStarting_bar,
							oxyPercStarting_fracDec,
							reqOxyFillPressure_bar);
	
	/* step 2: calculate top off pressure for air */
	calcPresAirTopOff(pressureTotalDesired_bar,
							pressureStarting_bar,
							reqOxyFillPressure_bar,
							reqAirTopUpPressure_bar);
	
	/* step 3: handle negative pressure / bleed off result */
	if (*reqOxyFillPressure_bar < 0)
	{
		/* gas in reservoir must be removed / bled off before mixing */
		/* TODO: do we have the mechanical capability to bleed off? 
		 * or do we throw an error / alarm in this case? */
	}
	
	/* step 4: range check outputs */
	/* TODO: range check outputs */
	
}

/**
 * @brief  calculate fill pressure for oxygen 
 *			Pf = (Pd*(Md-Mt) - Ps*(Ms-Mt)) / (Mf-Mt)
 * @param pressureTotalDesired_bar: The target pressure for the reservoir gas mix
 * @param oxyPercDesired_fracDec: Desired oxygen percentage as a fractional decimal. (100% = 1.0)
 * @param pressureStarting_bar: The pressure within the resevoir at the end of the 
 *			inhalation phase
 * @param oxyPercStarting_fracDec: The oxygen percentage within the reservoir at the 
 *			end of the inhalation phase as a fractional decimal
 * @param reqOxyFillPressure_bar: The required pressure of oxygen to add to the 
 *			reservoir to achieve desired mix in bar
 */
void JamCtrlGasMix::calcPresOxygenFill(const float * const pressureTotalDesired_bar,
											const float * const oxyPercDesired_fracDec,
											const float * const pressureStarting_bar,
											const float * const oxyPercStarting_fracDec,
											float * const reqOxyFillPressure_bar) 
{
	this->startingTerm = *pressureStarting_bar * (*oxyPercStarting_fracDec - this->mixAirSource_fracDec);
	this->desiredTerm = *pressureTotalDesired_bar * (*oxyPercDesired_fracDec - this->mixAirSource_fracDec);
	this->fillTerm = (this->mixOxygenSource_fracDec - this->mixAirSource_fracDec);
	*reqOxyFillPressure_bar = (this->desiredTerm - this->startingTerm) / this->fillTerm;
}

/* 
 * @brief calculate top off pressure for air
 *			Pt = Pd - Ps - Pf
 * @param pressureTotalDesired_bar: The target pressure for the reservoir gas mix
 * @param pressureStarting_bar: The pressure within the resevoir at the end of the 
 *			inhalation phase
 * @param reqOxyFillPressure_bar: The required pressure of oxygen to add to the 
 *			reservoir to achieve desired mix in bar
 * @param reqAirTopUpPressure_bar: The required pressure of air to add to the 
 *			reservoir to achieve desired mix in bar.
 * */
void JamCtrlGasMix::calcPresAirTopOff(const float * const pressureTotalDesired_bar,
										const float * const pressureStarting_bar,
										float * const reqOxyFillPressure_bar,
										float * const reqAirTopUpPressure_bar) 
{
	*reqAirTopUpPressure_bar = *pressureTotalDesired_bar - *pressureStarting_bar - *reqOxyFillPressure_bar;
}

