#include "Speaker.h"

/**
 * @brief Initializes GPIO for speaker output
 *        
 * @return int -- 0 on success, pigpio error code on error
 */
int Speaker::init() {
	int rc;
	
	// set GPIO pin as output
	rc = gpioSetMode(SPKR_GPIO, PI_OUTPUT);
	if (rc != 0) return rc;
	rc = gpioSetPullUpDown(SPKR_GPIO, PI_PUD_OFF);
	if (rc != 0) return rc;
	rc = gpioWrite(SPKR_GPIO, 0);
	if (rc != 0) return rc;
	
	return 0;
}

/**
 * @brief Starts a (square-wave) note playing through the speaker
 *        
 * @param freq -- frequency of note in Hz
 * @return int -- 0 on success, pigpio error code on error
 */
int Speaker::playNote(unsigned freq) {
	return gpioHardwarePWM(SPKR_GPIO, freq, 500E3);	// 50% duty cycle
}


/**
 * @brief Stops a note playing through the speaker
 *        
 * @return int -- 0 on success, pigpio error code on error
 */
int Speaker::stopNote() {
	return gpioHardwarePWM(SPKR_GPIO, 0, 0);
}