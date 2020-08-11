#ifndef __I2C_PRES_SENSOR_H__
#define __I2C_PRES_SENSOR_H__

#include "I2cSlaveDevice.h"


/**
 * @brief I2cAdc128D818 -- photonjamsim slave config i2c device
 * 
 * note: the simulation of this ADC handles ONLY adc channels
 *       and does not simulate any of the interrupt registers
 *       this is a simplified simulation designed to develop code against
 *       a simulated O2 sensor.
 * 
 *  as a result, do not expect the shutdown modes or any of the interrupts to function.
 * 
 *  also, initially there is no attempt at simulating the conversion rate, based on the
 *        number of enabled channels.
 *  this assumes continous conversion mode.
 * 
 *  the simulation will respond with the register contents to read back what was programmed.
 * 
 * NOTE: form data sheet:
 *   Channel Readings Registers (addresses 20h - 27h)
 *       The channel conversion readings are available in registers 20h to 27h. Each register is 16-bit wide to
 *        accommodate the 12-bit voltage reading or 9-bit temperature reading. Conversions can be read at any time and
 *        will provide the result of the last conversion. If a conversion is in progress while a communication is started, that
 *        conversion will be completed, and the Channel Reading Registers will not be updated until the communication is
 *        complete.
 */
class I2cAdc128D818 : public I2cSlaveDevice
{
public:
    I2cAdc128D818() = delete;
    /**
     * @brief Construct a new I2cSlaveDevice object
     * 
     * @param devAddr -- device address for this device.
     */
    I2cAdc128D818(unsigned devAddr) :
        I2cSlaveDevice(devAddr),
        _regConfig(0),
        _regInterruptStatus(0),
        _regInterrruptMask(0),
        _regConversionRate(0),
        _regChannelDisable(0),
        _regDeepShutdown(0),
        _regBusyStatus(0),
        _regMfgId(0x01),
        _regRevId(0x01)
        {
            memset(_regChannelRead,0,sizeof(_regChannelRead)/sizeof(*_regChannelRead));
            memset(_regLimit,0,sizeof(_regLimit)/sizeof(*_regLimit));
        };


    /**
     * @brief stop event received AFTER receiving a start event...
     * 
     * @param -- _rw -- read/write_
     */
    virtual void stop(unsigned _rw);

protected:

    // commands, see data sheet...
    //   https://www.ti.com/lit/ds/symlink/adc128d818.pdf?HQS=TI-null-null-digikeymode-df-pf-null-wwe&ts=1597004478353
    enum {
        REG_CFG = 0x00,         // configuration register
        REG_INTERRUPT_STATUS = 0x01,         // interrupt status register.
        REG_INTERRUPT_MASK = 0x03,         // interrupt mask register
        REG_CONVERSION_RATE = 0x07,         // conversion rate register
        REG_CHANNEL_DISABLE = 0x08,         // channel disable reigster
        REG_ONE_SHOT = 0x09,                // one shot register
        REG_DEEP_SHUTDOWN = 0x0a,           // deep shutdown register
        REG_ADVANCED_CONFIG = 0x0b,         // advanced configuration register
        REG_BUSY_STATUS = 0x0c,             // busy status register.
        REG_CHANNEL_READ = 0x20,            // channel read register(s) 0x20-0x27
        REG_LIMIT = 0x2a,                   // limit registers, 0x2a-0x39
        REG_MFG_ID = 0x3e,                  // manufacture id register
        REG_REVISION_ID = 0x3f,             // revision id.

        // simulation registers to do testing sim loopback testing
        REG_CHANNEL_WRITE =0x40             // write to the read channel 0x40-0x47
    };

    // configuration register bit masks...
    enum {
        CFG_START = (1<<0),         // start: 0: ADC128D818 in shutdown mode, 1: enable startup of monitoring operations
        CFG_INT_ENABLE = (1<<1),    // 1: enable hte interrupt output pin INT_
        CFG_INT_CLEAR = (1<<3),     // clear the interrupt outpin INT_, without affecting the contents of Interrupt Status 
                                    //   Registers. When this bit is set high, the device stops the 
                                    //   round-robin monitoring loop.
        CFG_INIT = (1<<7)           //  1: Restore default values to the following registers: Configuration, 
                                    //    Interrupt Status, Interrupt Read/Write Mask, Conversion Rate, 
                                    //    Channel Disable, One-Shot, Deep Shutdown, Advanced Configuration, 
                                    //    Busy Status, Channel Readings, Limit, Manufacturer ID, Revision ID. 
                                    //    This bit clears itself.
    };

    //
    // advanced configuration register, 
    //
    enum {
        ADV_CONFIG_EXT_REF_ENA = (1<<1),            // external reference enable
        ADV_CONFIG_MODE_0 = (0<<1), // mode 0
        ADV_CONFIG_MODE_1 = (1<<1), // mode 0
        ADV_CONFIG_MODE_2 = (2<<1), // mode 0
        ADV_CONFIG_MODE_3 = (3<<1), // mode 0
    };

    // busy status
    enum {
        BUSY_STATUS_BUSY = (1<<0),      // 1: ADC128d818 is converting
        BUSY_STATUS_NOT_READY = (1<<1)  // 1: waiting for power-up sequence to end
    };

    // register values
    unsigned _regConfig;
    unsigned _regInterruptStatus;
    unsigned _regInterrruptMask;
    unsigned _regConversionRate;
    unsigned _regChannelDisable;
    unsigned _regDeepShutdown;
    unsigned _regAdvancedConfig;
    unsigned _regBusyStatus;
    unsigned _regChannelRead[8];
    unsigned _regLimit[8];
    unsigned _regMfgId;
    unsigned _regRevId;





private:
};
#endif
