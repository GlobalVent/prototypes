#ifndef COMMMGRINTERFACE_H
#define COMMMGRINTERFACE_H

#include <QObject>

// Base interface on QObject to allow defining signals/slots.
class CommMgrInterface : public QObject
{
    Q_OBJECT
  public:
    using NumType = double;

    // Data in from system
    struct DataIn
    {
        // User input settings
        NumType fiO2;
        NumType tidalVol;
        NumType respRate;
        NumType ieRatio;
        NumType peep;

        // Valve state
        bool isAOpen;
        bool isBOpen;
        bool isCOpen;
        bool isDOpen;

        // Graph data
        NumType pressRes; // reservior pressure.
        NumType pressSys; // psys pressure
        NumType pressO2;  // pO2 sensor reading.
        NumType lungVol;  // lung volume

        DataIn() : fiO2{0.0}, tidalVol{0.0}, respRate{0.0},
            ieRatio{0.0}, peep{0.0}, isAOpen{false}, isBOpen{false},
            isCOpen{false}, isDOpen{false}, pressRes{0.0},
            pressSys{0.0}, pressO2{0.0}, lungVol{0.0} {};
    };

    CommMgrInterface() = default;
    virtual ~CommMgrInterface() = default;

    // Data from GUI to System
  public slots:
    virtual void onFio2Changed(int value) = 0;
    virtual void onTidalVolChanged(int value) = 0;
    virtual void onRespRateChanged(int value) = 0;
    virtual void onIeRatioChanged(int value) = 0;
    virtual void onPeepChanged(int value) = 0;

    virtual void onValveAOpenChanged(bool isOpen) = 0;
    virtual void onValveBOpenChanged(bool isOpen) = 0;
    virtual void onValveCOpenChanged(bool isOpen) = 0;
    virtual void onValveDOpenChanged(bool isOpen) = 0;

    // Data from System to GUI
  signals:
    void sigNewInData(DataIn);

  private:
};

// Declare as meta type so can use to pass data in signals/slots.
Q_DECLARE_METATYPE(CommMgrInterface::DataIn)
#endif /* COMMMGRINTERFACE_H */
