//
// test model with valve action AND plotting output...
//    we are going to record i pieces of data to plot with gnuplot
//    
//   time, Pres, Psys, Po2, Lvol, ValveA, ValveB, ValveC, ValvD
//
// NOTE: this does NOT run in real time..., it simulates it... so 
//       doing breakpoints does not throw things off...
//

#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <iomanip>
#include <iostream>
#include <strstream>
#include <istream>
#include <cmath>
#include <ostream>
#include <sstream>
#include <vector>


//#include "StrTokenize.h"
//#include "fmtstr.h"
#include "JamventSimModelRt.h"
#include "ParseTestArgs.h"
#include "TimeSeriesOutFile.h"

#ifndef TIME_UTC
#define TIME_UTC 0
#endif

using namespace std;

class TestSequence {
public:
	float runTime;
	bool  valveA;
	bool  valveB;
	bool  valveC;
	bool  valveD;
};

TestSequence testSequence[] = {
	{0.5, true,  false, false, false},
	{2, false, true,  false, false},
//	{0.5, false, true,  false, false},
	{1,   false, false, true,  false},
	{4,   false, false, false, true},

};

//vector::<TestSequence> testSequenceVect;



void printUsage() {
    cout << "testModel00 [options]" << endl 
         << "  --help -- print help" << endl
         << "  --verbose -- verbose mode" << endl
         << "  --outfile=<filename> -- output file plot test telemetry" 
         << "       files will have the filename.<testnum>.plt"
         << endl;
}

/**
 * @brief runModel for time interval then return.
 * 
 * @param model -- model to run.
 * @param timeStart -- timeZero to subtract off all recorded time.
 * @param nSteps -- number of detla time steps.
 * @param dt -- delta time
 * @param ts -- time series to record what is happening with the model
 * @return none
 */
void runModel(JamventSimModel &model, double timeStart, double nSteps, double dt, TimeSeriesMod &ts) {
	double t = timeStart;
	for (unsigned n = 0; n < nSteps; n++) {
		model.step(dt);
		t+= dt;
		ts.push_back(t, model.getPres(), model.getPsys(), model.getPO2(), model.getLvol(),
					model.getValveAopen(), model.getValveBopen(), model.getValveCopen(), model.getValveDopen());
	}
}
int  main(int argc, const char * argv []) {
    ParseTestArgs p(cerr);
    if (!p.parse(argc, argv))
        return(1);
    if (p.help) {
        printUsage();
        return(1);

    };
	string testName("testModel01");

    if (p.parms.size() > 0) {
        cout << "incorrect number of parameters" << endl;
        printUsage();
        return(1);
    }


	TimeSeriesMod ts;
	bool passed = true;
	cout << "Testing testing JamventSimModelRt ..." << endl;


	// note this is a test so it only runs for a short time and then stops...
	//  actual simulation on hardware would be a loop forever...

	// model test for real..
	JamventSimModelRt model;
	
	// note this test just makes sure the model itself does not crash.
	// but nothing yet is going on...
	//  should run for 20 seconds then exit
	model.init();
	ts.push_back(0, model.getPres(), model.getPsys(), model.getPO2(), model.getLvol(),
				 model.getValveAopen(), model.getValveBopen(), model.getValveCopen(), model.getValveDopen());


	double dt = 0.01;
	//double dt = 0.1;		// corser for easier debug
	double timeNow = 0;;
	unsigned nLoops = 5;
    TimeSeriesOutFile tout;
    if (!tout.open(p.outFileName, testName, dt, cerr))
        return(1);
	// for (auto t = testSequenceVect.begin(); t < testSequenceVect.end(); t++) {
	for (unsigned n=0; n < nLoops; n++) {
		for (auto t = testSequence; t < testSequence + sizeof(testSequence)/sizeof(*testSequence); t++) {
			model.setValveAopen(t->valveA);
			model.setValveBopen(t->valveB);
			model.setValveCopen(t->valveC);
			model.setValveDopen(t->valveD);
			cout << "timeNow=" << timeNow << ", "
				<< "runTime" << t->runTime << ", "
				<< "valveA=" << t->valveA << ", "
				<< "valveB=" << t->valveB << ", "
				<< "valveC=" << t->valveC << ", "
				<< "valveD=" << t->valveD 
				<< endl;
			runModel(model, timeNow, t->runTime/dt, dt, ts);
			timeNow+= t->runTime;
		}
	}

    tout.writeTimeSeries(ts);

	// kind of nothing to fail here	
	cout << "Test : " << (passed ? "PASSED" : "FAILED") << endl;

	return (passed ? 0 : 1);
}

