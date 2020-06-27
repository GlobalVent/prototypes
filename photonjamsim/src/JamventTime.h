/**
 * @file JamventTime.h
 * @author ralph bellofatto
 * @brief  Jamvent sim time module provides class to pass to jamventmodelrt that can be used 
 *         for getting the time as a double percision floating point.
 * @version 0.1
 * @date 2020-05-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef __JAMVENT_TIME_H__
#define __JAMVENT_TIME_H__

#include <chrono>

#if PARTICLE==1
#include <Particle.h>
#endif


class JamventTime {
public:
    JamventTime() {};
    ~JamventTime() {};
    /**
     * @brief get the time for NOW 
     * 
     * @return double 
     */
    double now() {
        #if PARTICLE==1
            noInterrupts();
            // millis rolls over after 49 days, so handle the roll over...
            static unsigned long lastMillis = 0;
            static unsigned long long totalTime = 0;
            unsigned long long deltaTime;
            unsigned long currMillis = millis();
            if (currMillis >= lastMillis)    
                deltaTime = currMillis-lastMillis;
            else   // we rolled over...
                deltaTime = ((unsigned long long)currMillis+0x100000000ULL)-lastMillis;
            lastMillis=currMillis;      // remember this.
            totalTime+= deltaTime;
            interrupts();
            return(1e-3*totalTime);
        #else

            using Clock = std::chrono::high_resolution_clock;
            using TimePoint = std::chrono::time_point<Clock>;
            // using Ms = std::chrono::milliseconds;
            // using Ns = std::chrono::nanoseconds;

            TimePoint point = Clock::now();
            const Clock::duration since_epoch = point.time_since_epoch();
            double ns = std::chrono::duration<double>(since_epoch).count();
            return(ns);
        #endif

    }
    void waitTime(float tw)
    {
        #if PARTICLE==1
        unsigned long ms = (unsigned long)(tw*1000);
        delay(ms);
        #else
            struct timespec timeReq;
            struct timespec rem;
            timeReq.tv_sec = (unsigned)tw;	// truncate the time  to get seconds.
            timeReq.tv_nsec = (unsigned)(tw*1e9)%1000000000ull;  // 10000 nano secionds (10 milliseconds)
            nanosleep(&timeReq, &rem);
        #endif
    }
    
protected:

private:

};


#endif

