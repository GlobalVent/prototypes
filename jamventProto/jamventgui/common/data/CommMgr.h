#ifndef COMMMGR_H
#define COMMMGR_H

#include <QTimer>

#include "CommMgrInterface.h"

#include "JamCtrlSim.h" // simulated version for now.

class CommMgr : public CommMgrInterface
{
    Q_OBJECT

  public:
    using DataIn = JamCtrlSim::DataIn;
    using InputNumType = JamCtrlSim::InputNumType;
    using GraphNumType = JamCtrlSim::GraphNumType;

    explicit CommMgr();
    virtual ~CommMgr();

    void start();
    void stop();

  public slots:
    void onFio2Changed(int value) override;
    void onTidalVolChanged(int value) override;
    void onRespRateChanged(int value) override;
    void onIeRatioChanged(int value) override;
    void onPeepChanged(int value) override;

    void onValveAOpenChanged(bool isOpen) override;
    void onValveBOpenChanged(bool isOpen) override;
    void onValveCOpenChanged(bool isOpen) override;
    void onValveDOpenChanged(bool isOpen) override;

  private:
    void onTimeout();

    JamCtrlSim m_jamCtrl; // sim setup for now...
    QTimer m_timer;       // Timer for polling communication
};
#endif /* COMMMGR_H */
