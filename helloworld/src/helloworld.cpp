/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Volumes/ct-ng/covid-19/github/prototypes/helloworld/src/helloworld.ino"
#include <stdint.h>
uint64_t microsNow();
void pinChangeInt();
void setup();
void loop();
#line 2 "/Volumes/ct-ng/covid-19/github/prototypes/helloworld/src/helloworld.ino"
SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(MANUAL);
int IOUT = D6;
int IIN  = D5;
int TOUT = A0;

/**
 * @brief get the total time in ms since we started.
 * 
 * @return total number of microseconds since we started.
 *          NOTE: this handles the roll over if called more than once an hour.
 */
uint64_t microsNow() {          // time in microseconds...
    static uint32_t lastNow_us=0;  // check for overflow.
    static uint64_t totalNow_us=0;
    uint64_t delta_us;
    uint32_t now_us = micros();
    if (now_us >= lastNow_us) 
        delta_us = now_us-lastNow_us;
    else                // we rolled over..
        delta_us = (0x100000000ull - lastNow_us) + now_us;
    lastNow_us = now_us;
    totalNow_us += delta_us;
    return(totalNow_us);
}
void pinChangeInt() {
    uint64_t start = microsNow();
    uint64_t end = start + 500;   // 1.5ms timeout
    digitalWriteFast(TOUT,1);
    detachInterrupt(IIN);
    pinReadFast(IIN);
    pinMode(IIN,  OUTPUT);
    pinMode(IIN,  INPUT_PULLUP);
    digitalWriteFast(TOUT,0);
    attachInterrupt(IIN, pinChangeInt, CHANGE,0);      

}
void setup() {
	Serial.begin(9600);
    microsNow();            // prime the pump...
    // set "led" pin to be an output
    pinMode(TOUT, OUTPUT);
    pinMode(IOUT, OUTPUT);
    pinMode(IIN,  INPUT_PULLUP);
    attachInterrupt(IIN, pinChangeInt, CHANGE,0);      

}
// LOOP function runs in repeating loop (after setup finishes)
unsigned cnt = 0;
void loop() {
    microsNow();            // and call this regularly... micros overflows every 71 minutes
    ATOMIC_BLOCK() {
        cnt++;
        digitalWriteFast(IOUT, cnt & 1);
    }
    delay(10);
}
