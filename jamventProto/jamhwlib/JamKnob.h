#ifndef __JAM_KNOB_H__
#define __JAM_KNOB_H__

/**
 * @brief JamKnob class
 *      this handles the rotary controller signals coming from the CPLD
 * 
 * internally this class uses callbacks from the pigpio library that
 * does callbacks whenever the io pins change, and keeps state internally
 * so that the caller will get a summary of what has transpired.
 * 
 * NOTE: if needed to make things simpler a notifier interface can be
 *       created to call back the user interface code informing it
 *       that something has changed.
 */
class JamKnob 
{
public:

	/**
	 * @brief knobButtonPressed -- return true if the knob button was pressed
	 *                             since the last time this was called.
	 * 
	 * @return true if the button was pressed, false if no press detected 
	 */
	virtual bool knobButtonPressed();

	/**
	 * @brief Return the number of knob clicks since the last time this was called.
	 * 
	 * @return number of knob turn clicks, < 0 is rotate left, > 0 is rotate right.
	 */
	virtual int knobClicks();

    
    /**
     * @brief Set the Gpio Pins for the rotary encoder
     * 
     * @param sw --switch pin (button down/up)
     * @param dt -- dt direction (lags clk by 90 degress phase shift to tell direction)
     * @param clk -- pulse pin to tell amount of rotation.
     * @return int 
     */
    virtual int setGpioPins(unsigned sw,  
                            unsigned dt,
                            unsigned clk);


protected:
private:

};
#endif
