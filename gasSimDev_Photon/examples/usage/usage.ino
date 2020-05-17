// Example usage for gasSimDev_photon library by AJ Ortiz, Ralph Bellofatto.

//AJO Notes -- first pass just using delay() calls for timing, can include
// SparkIntervalTimer library for more precise timing

//
//  main.cpp
//  xcode-test01
//
//  Created by ralph bellofatto on 4/12/20.
//  Copyright © 2020 ralph bellofatto. All rights reserved.
//
/*
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
*/
//#include "StrTokenize.h"
//#include "fmtstr.h"
#include "JamventSimModel.h"

#ifndef TIME_UTC
#define TIME_UTC 0
#endif

using namespace std;

typedef vector<string> ErrStrs;
/*
typedef std::pair<double, double> TimeSeriesPair_t;
typedef std::vector<TimeSeriesPair_t> VectorSeries_t;
typedef std::shared_ptr<VectorSeries_t> VectorSeriesPtr_t;
*/

// model test for real..
JamventSimModel model;
ErrStrs errs;

// AJO - Re-casting all to unsigned long (from double)
unsigned long timeLimit  = 5000;
unsigned long timeStart = millis(); //AJO Note - these used to be getCurrTime() calls
unsigned long timePrev = millis();  // time in
unsigned long timeNow = millis();
unsigned long timeEnd = timeStart+timeLimit;
bool passed = true;

/*
double getCurrTime() {
	struct timespec ts;
	//timespec_get(&ts, TIME_UTC);
    clock_gettime(CLOCK_REALTIME, &ts); // Works on Linux
	return(ts.tv_sec + ((double)ts.tv_nsec*1e-9));
}
*/

/**
 * @brief wait for a specfied number of seconds
 *        using nanosleep
 * @details time to wait for.
 *
 * @param tw time in seconds.
 */
void waitTime(double tw) {
	/*struct timespec timeReq;
	struct timespec rem;
	timeReq.tv_sec = (unsigned)tw;	// truncate the time  to get seconds.
	timeReq.tv_nsec = (unsigned)(tw*1e9);  // 10000 nano secionds (10 milliseconds)
	nanosleep(&timeReq, &rem); */ //AJO Commenting out and replacing w photon function
	delayMicroseconds(tw*1e-6);
}

void setup() {
	//cout << "Testing testing JamventSimModel ..." << endl;
Serial.begin(115200);
Serial.println("Testing testing JamventSimModel ...");
delay(5000);
	// note this is a test so it only runs for a short time and then stops...
	//  actual simulation on hardware would be a loop forever...
}

	// note this test just makes sure the model itself does not crash.
	// but nothing yet is going on...
	//  should run for 20 seconds then exit
void loop() {
    while (timeNow < timeEnd) {
		// AJ.  here is where you would inject changes in input into the model...
		//      and also read out pressure...
		//      you don't need to sleep, instead, you can just spin,
		//      but you will need a source of elapsed time...
		timeNow=millis();
		model.step(timeNow-timePrev);
		Serial.println("Continuing to next iteration...");
		Serial.println(timeNow);
		timePrev=timeNow;
	//	waitTime(0.01);		// wait 10 milliseconds...
	delay(10);
	}

	if (errs.size() > 0) {
		for (auto iter = errs.begin(); iter != errs.end(); iter++)
	//	Serial.println(iter); // was *iter in Ralph's cout call? - doesn't work AJO TO-DO - fix this
		Serial.println("TEST Failed");
		passed = false;
	}

passed = (passed) ? "PASSED" : "FAILED";
	Serial.print("Test : ");
	Serial.println(passed);
	Serial.println("Program complete. Pausing for 30 seconds...");
	delay(30000);
//	return (passed ? 0 : 1);
}
