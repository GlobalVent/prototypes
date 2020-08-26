#ifndef __JAM_CTRL_MGR_INTERFACE_H__
#define __JAM_CTRL_MGR_INTERFACE_H__

/**
 * @class JamCtrlMgrInteface
 * @brief Interface class for the control manager
 */
class JamCtrlMgrInterface
{
  public:
    using InputNumType = uint32_t; ///< Data type for input values
    using GraphNumType = double;   ///< Data type for graph values

    static constexpr GraphNumType BarPerCmh2o = 0.000980665;       ///< 0.000980665 Bar / 1 cmH2O
    static constexpr GraphNumType Cmh2oPerBar = 1.0 / BarPerCmh2o; ///< 1019.716 cmH2O / 1 Bar

    // Data in from the system.
    /**
    * @struct DataIn
    * @brief Data from system
    */
    struct DataIn
    {
        /// User input settings

        /// Oxygen to air mix
        /// Units: Percentage (%)
        /// Range: 21 to 100
        InputNumType fiO2_pc;

        /// Tidal volume
        /// Units: Milliliters
        /// Range: 10 to 1500
        InputNumType tidalVol_ml;

        /// Respiratory rate
        /// Units: None. Breaths per minute
        /// Range: 1 to 40
        InputNumType respRate;

        /// Inhale/Exhale ratio.
        /// Units: None. Choice of ratios
        /// Range: 0 (1:4) to 6 4:1)
        ///   0 = 1:4
        ///   1 = 1:3
        ///   2 = 1:2
        ///   3 = 1:1
        ///   4 = 2:1
        ///   5 = 3:1
        ///   6 = 4:1
        InputNumType ieRatio;

        /// Positive end expiratory pressure
        /// Units: Bar
        /// Range: 0.0 to 0.0196133 Bar (internal units), 0.0 to 20.0 cmH2O (display units)
        GraphNumType peep_bar;

        // Valve states.
        bool isAOpen; ///< True when valve A is open. False when closed.
        bool isBOpen; ///< True when valve B is open. False when closed.
        bool isCOpen; ///< True when valve C is open. False when closed.
        bool isDOpen; ///< True when valve D is open. False when closed.

        // Graph data
        /// Reservior pressure (high)
        /// Units: Bar
        /// Range: 0.0 Bar to 5.0 Bar
        GraphNumType pRes_bar; // Pressure in reservior

        /// System pressure (low)
        /// Units: Bar
        /// Range: 0.01 Bar to 1.2 Bar
        GraphNumType pSys_bar; // Pressure at the patient

        /// O2 percentage in mix
        /// Units: Percentage (%)
        /// Range: 21.0 to 100.0
        GraphNumType o2_pc;

        // @todo Fill in details.  Just a placeholder for now
        /// Lung volume
        GraphNumType lungVol_ml;

        DataIn() : fiO2_pc{0}, tidalVol_ml{0}, respRate{0},
                   ieRatio{0}, peep_bar{0.0}, isAOpen{false}, isBOpen{false},
                   isCOpen{false}, isDOpen{false}, pRes_bar{0.0},
                   pSys_bar{0.0}, o2_pc{0.0}, lungVol_ml{0.0} {};
    };

    JamCtrlMgrInterface() {};
    virtual ~JamCtrlMgrInterface() {};

    // Data from the system
    /**
    * @brief Get the latest data from the system.
    * @return Data from the system
    */
    virtual const DataIn getCtrlData() = 0;

    // Data out to the system.
    /**
    * @brief Set valve A open or closed
    * @param open True to open the valve. False to close.
    */
    virtual void setValveAopen(bool open) = 0;

    /**
    * @brief Set valve B open or closed
    * @param open True to open the valve. False to close.
    */
    virtual void setValveBopen(bool open) = 0;

    /**
    * @brief Set valve C open or closed
    * @param open True to open the valve. False to close.
    */
    virtual void setValveCopen(bool open) = 0;

    /**
    * @brief Set valve D open or closed
    * @param open True to open the valve. False to close.
    */
    virtual void setValveDopen(bool open) = 0;

    // Input elements. From GUI to the system.
    /**
    * @brief Set the Fraction of Inspired Oxygen (FiO2) value
    * @param fio2_pc New value for FiO2 in percent.
    *        See DataIn struct above for type and range.
    */
    virtual void setFio2(InputNumType fiO2_pc) = 0;

    /**
    * @brief Set the tidal volume value
    * @param tidalVol_ml New value for tidal volume in milliliters.
    *        See DataIn struct above for type and range.
    */
    virtual void setTidalVol(InputNumType tidalVol_ml) = 0;

    /**
    * @brief Set the respiratory rate value
    * @param respRate New value for respiritory rate in breaths per minute.
    *        See DataIn struct above for type and range.
    */
    virtual void setRespRate(InputNumType respRate) = 0;

    /**
    * @brief Set the Inspiration to Expiration Ratio (I:E) value
    * @param ieRatio New value for ratio.
    *        See DataIn struct above for type and range.
    */
    virtual void setIeRatio(InputNumType ieRatio) = 0;

    /**
    * @brief Set the Peak Expiratory End Pressure (PEEP) value
    * @param peep_bar New value for PEEP in Bar.
    *        See DataIn struct above for type and range.
    */
    virtual void setPeep(GraphNumType peep_bar) = 0;

    /**
    * @brief Convert a pressure value in Bars to cmH2O.
    * @param value_bar Value in Bars to convert.
    * @return Value converted to cmH2O.
    */
    static GraphNumType BarToCmh2o(GraphNumType value_bar)
    {
        return value_bar * Cmh2oPerBar;
    };

    /**
    * @brief Convert a pressure value in centimeters of H2O to Bars.
    * @param value_bar Value in centimeters of H2O to convert.
    * @return Value converted to Bars.
    */
    static GraphNumType Cmh2oToBar(GraphNumType value_cmh2o)
    {
        return value_cmh2o * BarPerCmh2o;
    }
};
#endif
