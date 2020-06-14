
#include "CommMgr.h"

namespace
{
    const qreal TimerInterval_ms = 100;
}

CommMgr::CommMgr()
    : QObject()
{
    QObject::connect(&m_timer, &QTimer::timeout, this, &CommMgr::onTimeout);
    m_timer.setInterval(TimerInterval_ms);
    m_timer.start();

    m_jamCtrl.setTimeInterval(0.01);
    m_jamCtrl.init(); // ontime init, after we do any other settings.
    m_jamCtrl.runThread();
}

CommMgr::~CommMgr()
{
    m_jamCtrl.killThread();
}

void CommMgr::onFio2Changed(NumType value)
{
}

void CommMgr::onTidalVolChanged(NumType value)
{
}

void CommMgr::onRespRateChanged(NumType value)
{
}

void CommMgr::onIeRatioChanged(NumType value)
{
}

void CommMgr::onPeepChanged(NumType value)
{
}

void CommMgr::onValveAOpenChanged(bool isOpen)
{
    m_jamCtrl.setValveAopen(isOpen);
}

void CommMgr::onValveBOpenChanged(bool isOpen)
{
    m_jamCtrl.setValveBopen(isOpen);
}

void CommMgr::onValveCOpenChanged(bool isOpen)
{
    m_jamCtrl.setValveCopen(isOpen);
}

void CommMgr::onValveDOpenChanged(bool isOpen)
{
    m_jamCtrl.setValveDopen(isOpen);
}

void CommMgr::onTimeout()
{
    JamCtrlData cd;

    cd = m_jamCtrl.getCtrlData();

    // qDebug() << "pRes = " << cd.pRes << "pSys = " << cd.pSys << "pO2 = " << cd .pO2 << "lVol = " << cd.lvol;

    // NOTE: scaling here is really artificial as these are not the data
    //       we are ultimatly graphing, but the data that we are just
    //       using to do a milestone.
    // JPW: Above note from Ralph.  New code below just passes on the data from the jamCtrl manager. Scaling happens when
    // data is applied to the graphs.

    DataIn newInData;

    // Graph data
    newInData.pressSys = cd.pSys;
    newInData.pressRes = cd.pRes;
    newInData.pressO2 = cd.pO2;
    newInData.lungVol = cd.lvol;

    // Valve data
    newInData.isAOpen = cd.valveAopen;
    newInData.isBOpen = cd.valveBopen;
    newInData.isCOpen = cd.valveCopen;
    newInData.isDOpen = cd.valveCopen;

    emit sigNewInData(newInData);
}
