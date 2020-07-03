#ifndef __GASSIM_CONV_H__
#define __GASSIM_CONV_H__

#include <algorithm>    


#include "GassimConst.h"

class GassimConv {
public:

	static double barToPascals(double bar) {
	    return(bar * 1e5);
	};
	static double pascalsToBar(double pa) {
    	return(pa/1e5);
    };

	static double pascalsToMb(double pa) {
    	return(pa/100);
    };
	static double mbToPascals(double mb) {
    	return(mb*100);
    };

	static double cToKelvin(double c) {
    	return(c+273.15);
    };

	static double kevlinToC(double k) {
    	return(k -273.15);
    };

	static double barToCmH2o(double bar) {
    	return(GassimConst::cmH20Bar*bar);
    };

	static double cmH2oToBar(double cmH20) {
    	return(cmH20/GassimConst::cmH20Bar);
    };

	static double lpmToLps(double lpm) {      // ## liters per minute to liters per second
    	return(lpm/60);    			   // bits/stdc++.h # listers per second to liters per minute
    };
    
	static double lpsToLpm(double lps) {
    	return(lps*60);
    };

	static double complianceCmH2oToBar(double c) {
		return(c*GassimConst::cmH20Bar);
	}
	static double complianceBarToCmH2o(double c) {
		return(c/GassimConst::cmH20Bar);
	}
protected:
private:
};

#endif
