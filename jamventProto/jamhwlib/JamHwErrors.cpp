#include "JamHwErrors.h"
#include <pigpio.h>

/**
* @brief Get description associated with an error code;
* classes which inherit from JamHwErrors and need to define
* their own error codes should have their own public enum
* with the error codes (named CLASS_NAME_ERROR_DESCRIPTION)
* starting in the -2000 range (as pigpio uses the -1000 range)
* and override this function to provide text for their own
* error codes, falling back on this one if it's not one of the
* class-specific ones.
* 
* See I2cAdcWithO2::getErrorText for an example of this.
* 
* @param err -- error code to get error text for
* @return std::string -- error text
*/
std::string JamHwErrors::getErrorText(int err) {
	std::string errStr;
	// currently this is a partial list of the errors
	// that are marked in the i2c section for pigpio.
	switch(err) {
	case PI_NOT_INITIALISED:  errStr = "function called before gpioInitialise"; break;
	case PI_BAD_HANDLE:       errStr = "unknown handle"; break;
	case PI_BAD_PARAM:		  errStr = "bad i2c/spi/ser parameter"; break;
	case PI_I2C_OPEN_FAILED:  errStr = "i2c open failed"; break;
	case PI_I2C_READ_FAILED:  errStr = "i2c read failed"; break;
	case PI_I2C_WRITE_FAILED: errStr = "i2c write failed"; break;
	case PI_BAD_I2C_ADDR:	  errStr = "bad i2c address"; break;
	case PI_BAD_FLAGS:		  errStr = "bad flags"; break;
	case PI_BAD_I2C_BUS:	  errStr = "bad i2c bus"; break;
	case PI_NO_HANDLE:		  errStr = "no handle available"; break;
	default:				  errStr = "Unknown pigpio error"; break;
	}
	return (errStr);
}
