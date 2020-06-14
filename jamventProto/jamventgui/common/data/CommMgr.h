#ifndef COMMMGR_H
#define COMMMGR_H

#include <QObject>
#include <QTimer>

#include "JamCtrlSim.h" // simulated version for now.

class CommMgr : public QObject
{
    Q_OBJECT
  public:
    using NumType = double;

    // Data in from system
    struct DataIn
    {
        // User input settings
        NumType fi02;
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
        NumType pressO2;  // po2 sensor reading.
        NumType lungVol;  // lung volume
    };

    explicit CommMgr();
    ~CommMgr();

    // JPW @todo. For now expose the controller for convenience.
    JamCtrlMgr *getJamCtrlMgr() { return &m_jamCtrl; };

    // Data from GUI to System
  public slots:
    void onFio2Changed(NumType value);
    void onTidalVolChanged(NumType value);
    void onRespRateChanged(NumType value);
    void onIeRatioChanged(NumType value);
    void onPeepChanged(NumType value);

    void onValveAOpenChanged(bool isOpen);
    void onValveBOpenChanged(bool isOpen);
    void onValveCOpenChanged(bool isOpen);
    void onValveDOpenChanged(bool isOpen);

    // Data from System to GUI
  signals:
    void sigNewInData(DataIn);

  private:
    void onTimeout();

    JamCtrlSim m_jamCtrl; // sim setup for now...
    QTimer m_timer;       // Timer for polling communication
};

// Declare as meta type so can use to pass data in signals/slots.
Q_DECLARE_METATYPE(CommMgr::DataIn)
#endif /* COMMMGR_H */
