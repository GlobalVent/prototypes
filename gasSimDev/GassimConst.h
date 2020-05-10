#ifndef __GASSIM_CONST__H__
#define __GASSIM_CONST__H__


/**
	units for the contstans...
	pressure:   bars
	flow rate:  l/s 
	volume:     liters
	weights:    grams/

 * 
 */

namespace GassimConst {
	static const double uGasConst=8.3144621e-2;   //# gas const in bars/liter
	static const double uGasConstCmH20=84.7840;   // # gas constant in cmH20/liter


	// molweights
	const static double molWeightAir = 28.9647;       // # grams per/mol
	static const double molWeightOxygen = 31.999;
	static const double molWeightNitrogen = 28.013;
	static const double molweightHydrogen = 4.124;

	const double boltzmannsConst = 1.381e-23;
	const double avogradoNumber = 6.022e23;       // # number of molecules in a mol

	const double cmH20Bar = 1019.7162129779;     //# how many cm s in 1 bar

	// # common resistances for our systems.
	// # resistance of mm pipe in bar/liters/s
	// #   This was calculated from some tables on the inernet.
	// #      and i think they are very wrong...
	const double reist6mmPipe=60;
	const double resist12mmPipe=4.86;
	const double reist18mmPipe=1.43;
	const double reist24mmPipe=0.619;

	// #
	// # resistance to get us a desired flow rate.
	// #   we need to be able to pressurize the container in
	// #   2 seconds.  for a respiration cycle
	// #     2 ltr container
	const double resist_5Lps=2;      // 0.5 l/s
	const double resist1Lps=1.0;
	const double resist2Lps=0.5;
	const double resist4Lps=0.25;

}



#endif
