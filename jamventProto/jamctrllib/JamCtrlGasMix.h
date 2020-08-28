#ifndef __JAM_CTRL_GAS_MIX_H__
#define __JAM_CTRL_GAS_MIX_H__

class JamCtrlGasMix 
{
public:
	JamCtrlGasMix() {};
	virtual ~JamCtrlGasMix() {}
	
	void initGasMix();


protected:	
	
private:
	/* The target pressure for the reservoir gas mix */
	float pressureDesired_bar;
	/* The pressure within the resevoir at the end of the inhalation phase */
	float pressureStarting_bar;
	/* The required pressure of oxygen to add to the reservoir to achieve desired mix */
	float pressureOxygen_bar;
	/* The required pressure of air to add to the reservoir to achieve desired mix. */
	float pressureAir_bar;
	
	/* Desired oxygen percentage as a fractional decimal. (100% = 1.0) */
	float mixDesired_fracDec;
	/* The oxygen percentage within the reservoir at the end of the inhalation phase
	 * as a fractional decimal */
	float mixStarting_fracDec;
	/* The fixed fraction of oxygen in the fill gas as a decimal
	 * TODO: does this need to be calibrated? */
	const float mixOxygenSource_fracDec = 1.0;
	/* The fixed fraction of oxygen in the top off gas as a decimal
	 * TODO: does this need to be calibrated? */
	const float mixAirSource_fracDec = 0.209;
	
	void getStartValues();
	void calcFractionOxygenFill();
	void calcFractionAirTopOff();
	void calcPresOxygenFill();
	void calcPresAirTopOff();
	
	
};
#endif
