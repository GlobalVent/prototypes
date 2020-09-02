#ifndef COMMMGRINTERFACE_H
#define COMMMGRINTERFACE_H

#include <QObject>

#include "JamCtrlMgr.h"

// Base interface on QObject to allow defining signals/slots.
class CommMgrInterface : public QObject
{
    Q_OBJECT
  public:
    CommMgrInterface() = default;
    virtual ~CommMgrInterface() = default;

    // Data from GUI to System
  public slots:
    virtual void onFio2Changed(int value) = 0;
    virtual void onTidalVolChanged(int value) = 0;
    virtual void onRespRateChanged(int value) = 0;
    virtual void onIeRatioChanged(int value) = 0;
    virtual void onPeepChanged(int value) = 0;

    virtual void onValveAO2OpenChanged(bool isOpen) = 0;
    virtual void onValveBAirOpenChanged(bool isOpen) = 0;
    virtual void onValveCInhaleOpenChanged(bool isOpen) = 0;
    virtual void onValveDExhaleOpenChanged(bool isOpen) = 0;

    // Data from System to GUI
  signals:
    void sigNewInData(JamCtrlMgr::DataIn);

  private:
};

// Declare as meta type so can use to pass data in signals/slots.
Q_DECLARE_METATYPE(JamCtrlMgr::DataIn)
#endif /* COMMMGRINTERFACE_H */
