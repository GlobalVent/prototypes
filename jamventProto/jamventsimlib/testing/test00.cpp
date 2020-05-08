//
//  main.cpp
//  xcode-test01
//
//  Created by ralph bellofatto on 4/12/20.
//  Copyright © 2020 ralph bellofatto. All rights reserved.
//

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
#include "GassimModel.h"

using namespace std;

typedef vector<string> ErrStrs;

// #
// # gasSrc <-> valveRin <-> resovior
// #  3bar        .1R         1 liter
// # pressures are in absolute pressure
// # temperature is 21c

// # time constant check
// # the end result of this after 1 second should be
// #    within acceptable rounding errors...
// # the same regardless of of dt
// #
// #



class SimpleGasModel: public GassimModel 
{
public:
	SimpleGasModel() {
		double pipeResist=0.1;
		double resPress = 1;
		// create the nodes
		_gasSrc = std ::make_shared<GassimSource>("gasSrc", 2.0, .21);
		_valveRin = std ::make_shared<GassimPipe>("valveRin", pipeResist, true);
		_reservoir = std::make_shared<GassimReservoir>("reservoir", resPress, 2);

		// add them to the mode.
		addNode(_gasSrc);
		addNode(_valveRin);
		addNode(_reservoir);

		// connect them up...
		connect(_gasSrc,_valveRin);
		connect(_valveRin,_reservoir);

	}
	// public container...
	GassimNode::NodePtr_t _gasSrc;
	GassimNode::NodePtr_t _valveRin;
	GassimNode::NodePtr_t _reservoir;

protected:
private:


};

typedef std::pair<double, double> TimeSeriesPair_t;
typedef std::vector<TimeSeriesPair_t> VectorSeries_t;
typedef std::shared_ptr<VectorSeries_t> VectorSeriesPtr_t;

VectorSeriesPtr_t doModel(double dt, 
	double timeLimit, 
	uint64_t stride)
{
	VectorSeriesPtr_t vp(make_shared<VectorSeries_t>());
	SimpleGasModel  model;
    uint64_t nsteps=0;
	double timeNow = 0.0;

    while (timeNow < timeLimit) {
        nsteps++;
	    // # strid the timesteps, so our number of
	    // #    samples are the same, so we can plot them
	    // #    together...
	    if((nsteps % stride) == 0) 	    {
	        model.step(dt*stride);
            model.next();
		}
	    vp->push_back(TimeSeriesPair_t(timeNow, model._reservoir->pressure()));
		timeNow += dt;

    }
	

	return(vp);
}
int  main(int argc, const char * argv []) {
	ErrStrs errs;
	bool passed = true;
	cout << "Testing test00 ..." << endl;
	
	double timelimit  = 4;
	VectorSeriesPtr_t vp = doModel(0.01, timelimit, 1);

#if 1
	// just blindly print out the vector...
	for (auto it = vp->begin(); it != vp->end(); it++) 
		cout << it->first <<  ", " << it->second << endl;
#endif
	if (errs.size() > 0) {
		for (auto iter = errs.begin(); iter != errs.end(); iter++) 
			cout << *iter << endl;
		cout << "TEST Failed" << endl;
		passed = false;
	}
	
	
	cout << "Test : " << (passed ? "PASSED" : "FAILED") << endl;

	return (passed ? 0 : 1);
}

