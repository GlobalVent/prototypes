//
//  main.cpp
//  xcode-test01
//
//  Created by ralph bellofatto on 4/12/20.
//  Copyright © 2020 ralph bellofatto. All rights reserved.
//

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


#include "StrTokenize.h"
#include "fmtstr.h"
#include "JamventSimModel.h"

using namespace std;

typedef vector<string> ErrStrs;

typedef std::pair<double, double> TimeSeriesPair_t;
typedef std::vector<TimeSeriesPair_t> VectorSeries_t;
typedef std::shared_ptr<VectorSeries_t> VectorSeriesPtr_t;


double getCurrTime() {
	struct timespec ts;
	timespec_get(&ts, TIME_UTC);
	return(ts.tv_sec + ((double)ts.tv_nsec*1e-9));
}
/**
 * @brief wait for a specfied number of seconds
 *        using nanosleep
 * @details time to wait for.
 * 
 * @param tw time in seconds.
 */
void waitTime(double tw) {
	struct timespec timeReq;
	timeReq.tv_sec = (unsigned)tw;	// truncate the time  to get seconds.
	timeReq.tv_nsec = (unsigned)(tw*1e9);  // 10000 nano secionds (10 milliseconds)
}

int  main(int argc, const char * argv []) {
	ErrStrs errs;
	bool passed = true;
	cout << "Testing testing JamventSimModel ..." << endl;
	double timeLimit  = 20;

	double timeStart = getCurrTime();
	double timePrev = getCurrTime();  // time in 
	double timeNow = getCurrTime();

	double timeEnd = timeStart+timeLimit;

	// note this is a test so it only runs for a short time and then stops...
	//  actual simulation on hardware would be a loop forever...

	// model test for real..
	JamventSimModel model;
	
	// note this test just makes sure the model itself does not crash.
	// but nothing yet is going on...
	//  should run for 20 seconds then exit
	while (timeNow < timeEnd) {
		// AJ.  here is where you would inject changes in input into the model...
		//      and also read out pressure...
		//      you don't need to sleep, instead, you can just spin, 
		//      but you will need a source of elapsed time...
		timeNow=getCurrTime();
		model.step(timeNow-timePrev);
		timePrev=timeNow;
		waitTime(0.01);		// wait 10 milliseconds...
	}

	if (errs.size() > 0) {
		for (auto iter = errs.begin(); iter != errs.end(); iter++) 
			cout << *iter << endl;
		cout << "TEST Failed" << endl;
		passed = false;
	}
	
	
	cout << "Test : " << (passed ? "PASSED" : "FAILED") << endl;

	return (passed ? 0 : 1);
}

