#include "I2cAdc128D818.h"

class I2cO2Sensor : public I2cAdc128D818 {
public:
    I2cO2Sensor() = delete;
    /**
     * @brief Construct a new I2cO2Sensor object
     * 
     * @param devAddr -- device address for this device.
     */
    I2cO2Sensor(unsigned devAddr) :
        I2cAdc128D818(devAddr) {};

protected:

private:

};