#ifndef __JAM_CTRL_MGR_H__
#define __JAM_CTRL_MGR_H__

#include <thread>

#include "JamventTime.h"
#include "JamCtrlMgrBase.h"

class JamCtrlMgr : public JamCtrlMgrBase
{
public:
    JamCtrlMgr() :
		_threadPtr(nullptr),
		_threadRunning(false),
		_log(nullptr),
		_killThread(false),
		_timeInterval(0.01),
		_testSeqIdx(0),
		_nextSeqTime(0)
		{};

    virtual ~JamCtrlMgr() {
		killThread();
	};

	/**
	 * @brief perform any onetime intiailiation needed/
	 * 
	 */
	virtual void init() {};

	/**
	 * @brief runThread
	 * @details run the thread, thread runs forever.
	 * 
	 * runs until kill is called (the destructor also implies kill).
	 */
	virtual void runThread();	
									
	/**
	 * @brief killThread -- kill the thread before exiting.
	 * @details kill the running thread.
	 */
	virtual void killThread();	

	/**
	 * @brief Check if the thread is running.
	 * @details checks if the thread is still running
	 *          
	 * @return true if the thread is running, false if not.
	 */
	virtual bool isThreadRunning();

	/**
	 * @brief Set the Time Interval to use for sampling inputs./
	 * 
	 * @param dt -- time interval to use.
	 */
	virtual void setTimeInterval(double dt) {
		_timeInterval = dt;
	}

	/**
	 * @brief setDebugLevel
	 * @details set the debug level to use for debugging 
	 *          must have done the setLogStream below
	 * 
	 * @param level  -- debug level to set, 0, no debug data. 1,2,3... increasing verbosity
	 */
	virtual void setDebugLevel(unsigned level);
	/**
	 * @brief setLogStream 
	 * @details set a logStream device to help with debugging...
	 * 
	 * @param ostr ostream object to send log data to.
	 */
	virtual void setLogStream(std::ostream *ostr); // set a log os stream device.  some place to send log info.

	/**
	 * @brief manually set the valve state
	 * @details manullly override the valve state for a given valve.
	 * 
	 * @param open [description]
	 */
	virtual void setValveAopen(bool open);
	virtual void setValveBopen(bool open);
	virtual void setValveCopen(bool open);
	virtual void setValveDopen(bool open);


	/**
	 * @brief get a copy of the controld data
	 * @details the contents of the control data in the system.
	 * @return [description]
	 */
	virtual const JamCtrlData getCtrlData() const;	// returns a copy of the control data.


protected:
    JamCtrlData _ctrlData;
	std::thread *_threadPtr;
	bool _threadRunning;
	bool _killThread;			// kill thread semiphore
	std::ostream *_log;
	JamventTime _simTime;	// low over high percision time functions
	double _timeInterval;

	virtual void step(double dt);

	void mainThread();
	void waitTime(float dw);

	unsigned _testSeqIdx;			// temporary test seqence index...
	double   _nextSeqTime;			// time to advance to the next sequence

private:

};


#endif 