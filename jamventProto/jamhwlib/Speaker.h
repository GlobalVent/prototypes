#ifndef __SPEAKER_H__
#define __SPEAKER_H__

#include <pigpio.h>

class Speaker {
public:
	static int init();
	static int playNote(unsigned freq);
	static int stopNote();
	
protected:
	// GPIO pin connected to audio amp: must be one of the hardware PWM pins (12, 13, 18, or 19)
	enum {
		SPKR_GPIO = 18
	};
};

#endif
