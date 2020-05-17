//
//
// step response for reservoire with small pressures.
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
#include <fstream>

#include "fmtstr.h"
#include "GassimModel.h"

#include "ParseTestArgs.h"
#include "TimeSeries.h"
#include "TimeSeriesOutFile.h"

using namespace std;

class ReservoirModel: public GassimModel 
{
public:
    ReservoirModel(float srcPress, float resist, float resPress, float resVolume) {
        _gasSrc = std ::make_shared<GassimSource>("gasSrc", srcPress, .21);
        _valveRin = std ::make_shared<GassimPipe>("valveRin", resist, true);
        _reservoir = std::make_shared<GassimReservoir>("reservoir", resPress, resVolume);

        // add them to the mode.
        addNode(_gasSrc);
        addNode(_valveRin);
        addNode(_reservoir);
        // connect them up...
        connect(_gasSrc,_valveRin);
        connect(_valveRin,_reservoir);

    };


    /**
     * @brief run
     *    run the gas model.
     *   
     * @param vp -- reference to vector of result pairs for the run
     * @param dt -- delta time to use. (time step in seconds) 
     * @param timeLimit -- time limint in seconds
     * @return true if successful, false if not.
     */
    bool run(TimeSeries &vp,
             double dt, 
             double timeLimit)
    {
        uint64_t nsteps=0;
        double timeNow = 0.0;

        next();
        vp.push_back(timeNow, _reservoir->pressure());
        while (timeNow <= timeLimit) {
            step(dt);
            next();
            timeNow += dt;
            vp.push_back(timeNow, _reservoir->pressure());
        }
        return(true);
    };

    double getResPO2() {
        return(_reservoir->pO2());
    }

protected:
    // public container...
    GassimNode::NodePtr_t _gasSrc;
    GassimNode::NodePtr_t _valveRin;
    GassimNode::NodePtr_t _reservoir;
private:
};

/**
 * @brief checkAtTime -- check a time series at time, for the expected value.
 * 
 * @param ts -- time series to check
 * @param time -- time to check at
 * @param exp  -- expected value
 * @param errs -- error stream to report errors on.
 * return -- returns additional count of errors.
 */
unsigned  checkAtTime(TimeSeries &ts, 
                 string const &testName,
                 double dt,
                 double time, 
                 double exp, 
                 ostream &errs) {
    auto it = ts.findTime(time, .00001);
    unsigned errCnt=0;
    if (.0005 < fabs(it->second-exp)) {
        errs << "(ERROR) " << testName << " "
             << __FUNCTION__ <<  " "
             << "TimeStep: "
             << floatw(3) << time << " "
             << "dt=" << dt << " "
             << "exp=" << exp << " "
             << "got=" << it->second
             << endl;
        errCnt++;
    }
    return(errCnt);
}
unsigned checkPo2(ReservoirModel &model, double dt, ostream &errs) {
    unsigned errCnt=0;
    if (.0005 < fabs(model.getResPO2()-.21) ) {
        errCnt++;
        errs << "(ERROR)" << "pO2 at dt=" << 0 <<  " "
             << "exp=" << "0.21" << " " 
             << "got=" << floatw
             (2) << model.getResPO2() 
             << endl;
    }
    return(errCnt);
}
/**
 * @brief 
 * 
 * @param outFileName 
 * @param errs stream to print errors to.
 * @return number of errors found.
 */
unsigned testInflate(string const &outFileName,
             double dt,
             double timeLimit,
             ostream &errs) 
{
    // #
    // # gasSrc <-> valveRin <-> resovior
    // #  2bar        1R         1 liter 1 bar...
    // # pressures are in absolute pressure
    // # temperature is 21c
    ofstream outFile;
    string testName(__FUNCTION__);
    unsigned errCnt=0;
    TimeSeriesOutFile tout;
    if (!tout.open(outFileName, testName, dt, errs))
        return(1);
    //  ok paraemters done
    cout << "# Testing " << testName << " dt=" << floatw(3) << dt << endl;
    TimeSeries vp;
    ReservoirModel  model(1.04,  // srcPress
                          1,  // resist
                          1,  // resPress
                          1); // resVolume
    model.init();
    errCnt+=checkPo2(model, 0, errs);
    model.run(vp, dt, timeLimit);
    errCnt+=checkAtTime(vp, testName, dt, 1.0, 1+0.632, errs);
    errCnt+=checkAtTime(vp, testName, dt, 2.0, 1+0.865, errs);
    errCnt+=checkAtTime(vp, testName, dt, 3.0, 1+0.950, errs);
    errCnt+=checkAtTime(vp, testName, dt, 4.0, 1+0.982, errs);
    errCnt+=checkPo2(model, timeLimit, errs);

    tout.writeTimeSeries(vp);

    return(errCnt);

}

/**
 * @brief testDeflate test discharge of resoivoire to source
 * 
 * @param outFileName 
 * @param dt 
 * @param timeLimit 
 * @param errs 
 * @return number of errors found.
 */
unsigned testDeflate(string const &outFileName,
             double dt,
             double timeLimit,
             ostream &errs) 
{
    // #
    // # gasSrc <-> valveRin <-> resovior
    // #  1 bar        1R         2 liter 2 bar
    // # pressures are in absolute pressure
    // # temperature is 21c
    string testName(__FUNCTION__);
    ofstream outFile;
    unsigned errCnt=0;
    TimeSeriesOutFile tout;
    if (!tout.open(outFileName, testName, dt, errs))
        return(1);
    //  ok paraemters done
    cout << "# Testing " << testName << " dt=" << floatw(3) << dt << endl;
    TimeSeries vp;
    
    ReservoirModel  model(1,  // srcPress
                          1,  // resist
                          1.04,  // resPress
                          1); // resVolume
    model.init();
    errCnt+=checkPo2(model, 0, errs);
    model.run(vp, dt, timeLimit);
    errCnt+=checkAtTime(vp, testName, dt, 1.0, 2-0.632, errs);
    errCnt+=checkAtTime(vp, testName, dt, 2.0, 2-0.865, errs);
    errCnt+=checkAtTime(vp, testName, dt, 3.0, 2-0.950, errs);
    errCnt+=checkAtTime(vp, testName, dt, 4.0, 2-0.982, errs);
    errCnt+=checkPo2(model, timeLimit, errs);

    tout.writeTimeSeries(vp);
    return(errCnt);
}

void printUsage() {
    cout << "test00 [options]" << endl 
         << "  --help -- print help" << endl
         << "  --verbose -- verbose mode" << endl
         << "  --outfile=<filename> -- output file plot test telemetry" 
         << "       files will have the filename.<testnum>.plt"
         << endl;
}


int  main(int argc, const char * argv []) {
    ostringstream errs;
    bool passed = true;

    ParseTestArgs p(cerr);
    if (!p.parse(argc, argv))
        return(1);
    if (p.help) {
        printUsage();
        return(1);

    };

    if (p.parms.size() > 0) {
        cout << "incorrect number of parameters" << endl;
        printUsage();
        return(1);
    }

    unsigned errCnt = 0;
    // first test, timestep of 1 second... we should get 1-(1/e) (.632)
    errCnt+=testInflate(p.outFileName, 1,    6, errs);
    errCnt+=testInflate(p.outFileName, .001, 6, errs);
    
    errCnt+=testDeflate(p.outFileName, 1,    6, errs);
    errCnt+=testDeflate(p.outFileName, .001, 6, errs);
    
    if (errs.str().size() > 0) {
        cout << errs.str();
        cout << "TEST Failed" << endl;
        passed = false;
    }
    
    cout << "# Test : " << (passed ? "PASSED" : "FAILED") << endl;

    return (passed ? 0 : 1);
}

