
/**
 * @file testCtrl01.cpp
 * @author RB
 * @brief 
 * @version 0.1
 * @date 2020-05-23
 * 
 * @copyright Copyright (c) 2020
 * 
 * simple test to do blind valve cycling and get telemetry back to the user.
 * 
 * 
 */
#include <unistd.h>
#include <iostream>
#include "ParseTestArgs.h"
#include "TimeSeriesOutFile.h"
#include "JamventTime.h"

#include "JamCtrlSim.h"

using namespace std;

typedef vector<string> ErrStrs;

void printUsage() {
    cout << "testCtrl01 [options]" << endl 
         << "  --help -- print help" << endl
         << "  --verbose -- verbose mode" << endl
         << "  --outfile=<filename> -- output file plot test telemetry" 
         << "       files will have the filename.<testnum>.plt"
         << endl;
}


int main(int argc, const char * argv []) {
    ParseTestArgs p(cerr);
    if (!p.parse(argc, argv))
        return(1);
    if (p.help) {
        printUsage();
        return(1);

    };
	string testName("testCtrl01");

    if (p.parms.size() > 0) {
        cout << "incorrect number of parameters" << endl;
        printUsage();
        return(1);
    }
    TimeSeriesOutFile tout;
    if (!tout.open(p.outFileName, testName, 0.01, cerr))
        return(1);

	ErrStrs errs;
	bool passed = true;
	JamCtrlSim jamCtrlSim;
	JamventTime jamTime;

	TimeSeriesMod ts;

	cout << "Testing jamCtlSim functions...xx" << endl;

	jamCtrlSim.setLogStream(&cout);
	jamCtrlSim.setTimeInterval(0.01);

	jamCtrlSim.init();		// ontime init, after we do any other settings.

    JamCtrlSim::DataIn cd;
	cd = jamCtrlSim.getCtrlData();
	ts.push_back(0, cd.pRes_bar, cd.pSys_bar, cd.o2_pc, cd.lungVol_ml,
				 cd.valveAO2Open, cd.valveBAirOpen, cd.valveCInhaleOpen, cd.valveDExhaleOpen);

	// finite time...
	double timeStart = jamTime.now();
	double timeEnd = timeStart + 30;
	double timeNow = timeStart;
	jamCtrlSim.runThread();
	while ((timeNow=jamTime.now()) < timeEnd) {
		jamTime.waitTime(0.10); // sample us 10 times a second.
		cd = jamCtrlSim.getCtrlData();
		ts.push_back(timeNow-timeStart, cd.pRes_bar, cd.pSys_bar, cd.o2_pc, cd.lungVol_ml,
					cd.valveAO2Open, cd.valveBAirOpen, cd.valveCInhaleOpen, cd.valveDExhaleOpen);
	}
	jamCtrlSim.killThread();

    tout.writeTimeSeries(ts);

	if (errs.size() > 0) {
		for (auto iter = errs.begin(); iter != errs.end(); iter++) 
			cout << *iter << endl;
		cout << "TEST Failed" << endl;
		passed = false;
	}
	
	cout << "Test : " << (passed ? "PASSED" : "FAILED") << endl;

	return (passed ? 0 : 1);
}

