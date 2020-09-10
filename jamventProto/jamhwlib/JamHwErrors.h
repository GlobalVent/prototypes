#ifndef __JAM_HW_ERRORS_H__
#define __JAM_HW_ERRORS_H__

#include <string>

/**
 * @brief JamHwErrors
 * 
 * Generic error-handling functions to be used by JamHwLib
 * base classes, which are all supposed to inherit from it
 */
class JamHwErrors {
public:
	std::string getErrorText(int err);
};

#endif
