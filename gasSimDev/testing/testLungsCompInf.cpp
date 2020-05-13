//
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

class LungsModel: public GassimModel 
{
public:
    LungsModel(float srcPress, 
               float resist,  
               float lungsPress, 
               float lungsVolume, 
               float compliance) {
        _gasSrc = std ::make_shared<GassimSource>("gasSrc", srcPress, 1.0);  // 100% O2
        _valveRin = std ::make_shared<GassimPipe>("valveRin", resist, true);
        _lungs = std::make_shared<GassimLungs>("lungs", lungsPress, lungsVolume, compliance);

        // add them to the mode.
        addNode(_gasSrc);
        addNode(_valveRin);
        addNode(_lungs);
        // connect them up...
        connect(_gasSrc,_valveRin);
        connect(_valveRin,_lungs);

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
    bool run(TimeSeriesPVO2 &vp,
             double dt, 
             double timeLimit)
    {
        uint64_t nsteps=0;
        double timeNow = 0.0;

        next();       
        vp.push_back(timeNow, _lungs->pressure(), _lungs->volume(), _lungs->pO2());
        while (timeNow <= timeLimit) {
            step(dt);
            next();
            timeNow += dt;
            vp.push_back(timeNow, _lungs->pressure(),_lungs->volume(), _lungs->pO2());
        }
        return(true);
    };

    void setSrcPO2(double pO2) {
        _gasSrc->setPO2(pO2);      // start out with NO o2, and inject 100% 02
    }

    double getLungsPO2() {
        return(_lungs->pO2());
    }
    void setLungsPO2(double pO2) {
        _lungs->setPO2(pO2);      // start out with NO o2, and inject 100% 02
    }

protected:
    // public container...
    GassimNode::NodePtr_t _gasSrc;
    GassimNode::NodePtr_t _valveRin;
    GassimNode::NodePtr_t _lungs;
private:
};

/**
 * @brief checkPres -- check a time series at time, for the expected value.
 * 
 * @param ts -- time series to check
 * @param time -- time to check at
 * @param exp  -- expected value
 * @param errs -- error stream to report errors on.
 * return -- returns additional count of errors.
 */
unsigned  checkPres(TimeSeriesPVO2 &ts, 
                 string const &testName,
                 double dt,
                 double time, 
                 double exp, 
                 ostream &errs) {
    auto it = ts.findTime(time, .00001);
    unsigned errCnt=0;
    if ((isnan(it->p) ) || (.0005 < fabs(it->p-exp))) {
        errs << "(ERROR) " << testName << " "
             << __FUNCTION__ <<  " "
             << "TimeStep: "
             << floatw(3) << time << " "
             << "dt=" << dt << " "
             << "exp=" << exp << " "
             << "got=" << it->p
             << endl;
        errCnt++;
    }
    return(errCnt);
}

/**
 * @brief checkVol -- check volume at dt
 * 
 * @param ts -- time series to check
 * @param testName -- test name to display
 * @param time -- time to check at
 * @param exp  -- expected value
 * @param errs -- error stream to report errors on.
 * @return -- returns additional count of errors.
 */
unsigned checkVol(TimeSeriesPVO2 &ts, 
                     string const &testName,
                     double dt,
                     double time, 
                     double exp, 
                     ostream &errs) {
    auto it = ts.findTime(time, .00001);
    unsigned errCnt=0;
    if ((isnan(it->v) ) || (.0005 < fabs(it->v-exp))) {
        errs << "(ERROR) " << testName << " "
             << __FUNCTION__ <<  " "
             << "TimeStep: "
             << floatw(3) << time << " "
             << "dt=" << dt << " "
             << "exp=" << exp << " "
             << "got=" << it->v
             << endl;
        errCnt++;
    }
    return(errCnt);
}

/**
 * @brief checkPo2 at time -- check a time series at time, for the expected value.
 * 
 * @param ts -- time series to check
 * @param time -- time to check at
 * @param exp  -- expected value
 * @param errs -- error stream to report errors on.
 * return -- returns additional count of errors.
 */
unsigned  checkPo2(TimeSeriesPVO2 &ts, 
                 string const &testName,
                 double dt,
                 double time, 
                 double exp, 
                 ostream &errs) {
    auto it = ts.findTime(time, .00001);
    unsigned errCnt=0;
    if (.0005 < fabs(it->o2-exp)) {
        errs << "(ERROR) " << testName << " "
             << __FUNCTION__ <<  " "
             << "TimeStep: "
             << floatw(3) << time << " "
             << "dt=" << dt << " "
             << "exp=" << exp << " "
             << "got=" << it->o2
             << endl;
        errCnt++;
    }
    return(errCnt);
}

/**
 * @brief testInflateComp0
 * 
 * @param outFileName 
 * @param dt 
 * @param timeLimit 
 * @param compliance 
 * @param errs 
 * @return unsigned 
 */
unsigned testInflate(string const &outFileName,
                         double dt,
                         double timeLimit,
                         ostream &errs) 
{
    // #
    // # gasSrc <-> valveRin <-> lungs
    // #  2bar        1R         1 liter 1 bar... compiliance 0
    // # pressures are in absolute pressure
    // # temperature is 21c
    //
    // # compliance of zero should behave the same as a fixed size reservoir
    ofstream outFile;
    string testName(__FUNCTION__);
    unsigned errCnt=0;
    TimeSeriesOutFile tout;
    if (!tout.open(outFileName, testName, dt, errs))
        return(1);

    //  ok paraemters done
    cout << "# Testing " << testName << " dt=" << float(3) << dt << endl;
    TimeSeriesPVO2 vp;
    LungsModel  model(2,  // srcPress
                      1,  // resist
                      1,  // lungsPressure
                      1,  // lungsVoume
                      INFINITY); // compliance
    model.setLungsPO2(0);                      
    model.run(vp, dt, timeLimit);

    errCnt+=checkPres(vp, testName, dt, 1.0, 1,   errs);
    errCnt+=checkVol (vp, testName, dt, 1.0, 1+1, errs);
    errCnt+=checkPo2 (vp, testName, dt, 1.0, 1.0/2, errs);

    errCnt+=checkPres(vp, testName, dt, 2.0, 1,   errs);
    errCnt+=checkVol (vp, testName, dt, 2.0, 1+2, errs);
    errCnt+=checkPo2 (vp, testName, dt, 2.0, 2.0/3, errs);

    errCnt+=checkPres(vp, testName, dt, 3.0, 1,   errs);
    errCnt+=checkVol (vp, testName, dt, 3.0, 1+3, errs);
    errCnt+=checkPo2 (vp, testName, dt, 3.0, 3.0/4, errs);

    errCnt+=checkPres(vp, testName, dt, 4.0, 1,   errs);
    errCnt+=checkVol (vp, testName, dt, 4.0, 1+4, errs);
    errCnt+=checkPo2 (vp, testName, dt, 4.0, 4.0/5, errs);


    tout.writeTimeSeries(vp);
    return(errCnt);

}

/**
 * @brief testInflateFrom0
 * 
 * @param outFileName 
 * @param dt 
 * @param timeLimit 
 * @param compliance 
 * @param errs 
 * @return unsigned 
 */
unsigned testInflateFrom0(string const &outFileName,
                         double dt,
                         double timeLimit,
                         ostream &errs) 
{
    // #
    // # gasSrc <-> valveRin <-> lungs
    // #  2bar        1R         1 liter 1 bar... compiliance 0
    // # pressures are in absolute pressure
    // # temperature is 21c
    //
    // # compliance of zero should behave the same as a fixed size reservoir
    ofstream outFile;
    string testName(__FUNCTION__);
    unsigned errCnt=0;
    TimeSeriesOutFile tout;
    if (!tout.open(outFileName, testName, dt, errs))
        return(1);

    //  ok paraemters done
    cout << "# Testing " << testName << " dt=" << float(3) << dt << endl;
    TimeSeriesPVO2 vp;
    LungsModel  model(2,  // srcPress
                      1,  // resist
                      1,  // lungsPressure  
                      0,  // lungsVoume
                      INFINITY); // compliance
    model.setLungsPO2(1);  
    model.setSrcPO2(0.5);

    model.run(vp, dt, timeLimit);


    errCnt+=checkPres(vp, testName, dt, 1.0, 1,   errs);
    errCnt+=checkVol (vp, testName, dt, 1.0, 0+1, errs);
    errCnt+=checkPo2 (vp, testName, dt, 1.0, 0.5, errs);

    errCnt+=checkPres(vp, testName, dt, 2.0, 1,   errs);
    errCnt+=checkVol (vp, testName, dt, 2.0, 0+2, errs);
    errCnt+=checkPo2 (vp, testName, dt, 2.0, 0.5, errs);

    errCnt+=checkPres(vp, testName, dt, 3.0, 1,   errs);
    errCnt+=checkVol (vp, testName, dt, 3.0, 0+3, errs);
    errCnt+=checkPo2 (vp, testName, dt, 3.0, 0.5, errs);

    errCnt+=checkPres(vp, testName, dt, 4.0, 1,   errs);
    errCnt+=checkVol (vp, testName, dt, 4.0, 0+4, errs);
    errCnt+=checkPo2 (vp, testName, dt, 4.0, 0.5, errs);


    tout.writeTimeSeries(vp);
    return(errCnt);

}

/**
 * @brief testDeflateComp0 test discharge of Lungs to source
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
    string testName(__FUNCTION__);
    ofstream outFile;
    unsigned errCnt=0;
    TimeSeriesOutFile tout;
    if (!tout.open(outFileName, testName, dt, errs))
        return(1);
    //  ok paraemters done
    cout << "# Testing " << testName << " dt=" << float(3) << dt << endl;
    TimeSeriesPVO2 vp;
    LungsModel  model(1,  // srcPress
                      1,  // resist
                      2,  // lungsPressure
                      4,  // lungsVoume
                      INFINITY); // compliance
    model.setLungsPO2(0.5);
    model.run(vp, dt, timeLimit);
    errCnt+=checkPres(vp, testName, dt, 1.0, 2,   errs);
    errCnt+=checkVol (vp, testName, dt, 1.0, 3,   errs);
    errCnt+=checkPo2 (vp, testName, dt, 1.0, 0.5, errs);

    errCnt+=checkPres(vp, testName, dt, 2.0, 2,   errs);
    errCnt+=checkVol (vp, testName, dt, 2.0, 2,   errs);
    errCnt+=checkPo2 (vp, testName, dt, 2.0, 0.5, errs);

    errCnt+=checkPres(vp, testName, dt, 3.0, 2,   errs);
    errCnt+=checkVol (vp, testName, dt, 3.0, 1,   errs);
    errCnt+=checkPo2 (vp, testName, dt, 3.0, 0.5, errs);

    errCnt+=checkPres(vp, testName, dt, 4.0, 2,   errs);
    errCnt+=checkVol (vp, testName, dt, 4.0, 0,   errs);
    errCnt+=checkPo2 (vp, testName, dt, 4.0, 0.5, errs);

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
    errCnt+=testInflateFrom0(p.outFileName, 1,    6, errs);

    if (errs.str().size() > 0) {
        cout << errs.str();
        cout << "TEST Failed" << endl;
        passed = false;
    }
    
    cout << "# Test : " << (passed ? "PASSED" : "FAILED") << endl;

    return (passed ? 0 : 1);
}

