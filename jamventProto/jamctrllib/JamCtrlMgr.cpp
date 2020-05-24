

#include <unistd.h>
#include <thread>

#include "fmtstr.h"
#include "JamCtrlMgr.h"
#include "JamventTime.h"

using namespace std;

class TestSequence {
public:
	float runTime;
	bool  valveA;
	bool  valveB;
	bool  valveC;
	bool  valveD;
};

// temporary test sequence for initial hookup
//    replace with real model later..
TestSequence testSequence[] = {
	{1, true,  false, false, true},  // charge o2 and exhale
	{1, false, true,  false, true},  // charge air and exhale
	{1, false, false, true,  false}, // inhale
};

/**
 * @brief runThread
 * @details run the thread, thread runs forever.
 * 
 * runs until kill is called (the destructor also implies kill).
 */
void JamCtrlMgr::runThread() 
{
    if (_threadPtr)
        return;
    _threadPtr  = new std::thread(&JamCtrlMgr::mainThread, this);
}
                                
/**
 * @brief killThread -- kill the thread before exiting.
 * @details kill the running thread.
 */
void JamCtrlMgr::killThread()
{
    if (! _threadPtr) 
        return;
    _killThread = true;
    if (_threadPtr->joinable())
        _threadPtr->join();
    delete _threadPtr, _threadPtr = nullptr;
}

void JamCtrlMgr::step(double dt) 
{
    auto t = testSequence + _testSeqIdx;
    double timeNow=_simTime.now();
    if ((_nextSeqTime==0) || (_nextSeqTime <= timeNow)) {
        setValveAopen(t->valveA);
		setValveBopen(t->valveB);
		setValveCopen(t->valveC);
		setValveDopen(t->valveD);
if (_log) *_log 
    << "setValves: " << timeNow << " " 
    << t->valveA << ", "
    << t->valveB << ", "
    << t->valveC << ", "
    << t->valveD 
    << endl;
        _nextSeqTime = timeNow + t->runTime;
        _testSeqIdx++;
        if (_testSeqIdx >= sizeof(testSequence)/sizeof(*testSequence))
            _testSeqIdx = 0;
    }

}
/**
 * @brief mainThread -- main therad called from thread lambda function.
 * 
 */
void JamCtrlMgr::mainThread() 
{

    if (_log) 
        *_log << "starting thread." << endl << flush;
    step(0);        // do our initial condition step...
    double timePrev=0;
    while (! _killThread) {
        double timeNow=_simTime.now();
        if (timePrev == 0) 
            timePrev=timeNow-_timeInterval;
        double deltaTime = timeNow - timePrev;

        step(deltaTime);

        timeNow=_simTime.now();         // recalucate due to the time we spent in the step function.
        deltaTime=timeNow-timePrev;
        double timeLeftToDt = (timePrev+_timeInterval*2)-timeNow;   // this should be in the future.
        // compute the time we shoud sleep, given all of the delays to date...
        double dt;
        if (timeLeftToDt > 0)
            dt = timeLeftToDt;
        else
            dt = 0;

        
        timePrev=timeNow;
        if (dt > 0)
            waitTime(dt);
    }
    if (_log) 
        *_log << "Thread done" << endl << flush;
}

/**
 * @brief waitTime -- wait for a specified number of nano seconds by going to sleep
 * 
 * @param tw -- time in seconds to wait (.001 is 1ms)
 */
void JamCtrlMgr::waitTime(float tw)
{
	struct timespec timeReq;
	struct timespec rem;
	timeReq.tv_sec = (unsigned)tw;	// truncate the time  to get seconds.
	timeReq.tv_nsec = (unsigned)(tw*1e9)%1000000000ull;  // 10000 nano secionds (10 milliseconds)
	nanosleep(&timeReq, &rem);
}

/**
 * @brief Check if the thread is running.
 * @details checks if the thread is still running
 *          
 * @return true if the thread is running, false if not.
 */
bool JamCtrlMgr::isThreadRunning()
{
    if (! _threadPtr)
        return(false);
    return(_threadPtr->joinable());
}


/**
 * @brief setDebugLevel
 * @details set the debug level to use for debugging 
 *          must have done the setLogStream below
 * 
 * @param level  -- debug level to set, 0, no debug data. 1,2,3... increasing verbosity
 */
void JamCtrlMgr::setDebugLevel(unsigned level) 
{

}
/**
 * @brief setLogStream 
 * @details set a logStream device to help with debugging...
 * 
 * @param ostr ostream object to send log data to.
 */
void JamCtrlMgr::setLogStream(std::ostream *ostr)  
{
    _log = ostr;
}

/**
 * @brief manually set the valve state.
 * @details manullly override the valve state for a given valve.
 * 
 * @param open [description]
 */
void JamCtrlMgr::setValveAopen(bool open) 
{

}
void JamCtrlMgr::setValveBopen(bool open)
{
    
}
void JamCtrlMgr::setValveCopen(bool open)
{
    
}
void JamCtrlMgr::setValveDopen(bool open)
{
    
}

/**
 * @brief get a copy of the controld data
 * @details the contents of the control data in the system.
 * @return [description]
 */
const JamCtrlData JamCtrlMgr::getCtrlData()	const 
{
    return(_ctrlData);
}
