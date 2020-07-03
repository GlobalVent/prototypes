#include <QDebug>

#include "ConfigJson.h"
#include "CommMgr.h"

namespace
{
}

CommMgr::CommMgr()
{
    QObject::connect(&m_timer, &QTimer::timeout, this, &CommMgr::onTimeout);
}

CommMgr::~CommMgr()
{
    m_timer.stop();
    m_jamCtrl.killThread();
}

void CommMgr::start()
{
    m_jamCtrl.setTimeInterval(0.01);
    m_jamCtrl.init(); // ontime init, after we do any other settings.
    m_jamCtrl.runThread();

    const ConfigJson::ConfigData &configData = ConfigJson::Instance().getConfigData();
    m_timer.setInterval(configData.pollingTimeout_ms);
    m_timer.start();
}

void CommMgr::stop()
{
    m_timer.stop();
    m_jamCtrl.killThread();
}

void CommMgr::onFio2Changed(int value)
{
    // JPW @todo. Send out new value to system.
}

void CommMgr::onTidalVolChanged(int value)
{
}

void CommMgr::onRespRateChanged(int value)
{
}

void CommMgr::onIeRatioChanged(int value)
{
}

void CommMgr::onPeepChanged(int value)
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

    // qDebug() << "CommMgr::onTimeout(): pRes = " << cd.pRes << "pSys = " << cd.pSys << "pO2 = " << cd .pO2 << "lVol = " << cd.lvol;

    // NOTE: scaling here is really artificial as these are not the data
    //       we are ultimatly graphing, but the data that we are just
    //       using to do a milestone.

    // JPW: Above note from Ralph.  New code below just passes on the data from the jamCtrl manager. Scaling happens when
    // data is applied to the graphs.

    DataIn newInData;

    // Graph data
    newInData.pressSys = cd.pSys / 3.0;  // Massage to match simulator.
    newInData.pressRes = (cd.pRes / 1000) - 1.0; // Massage to match simulator.
    newInData.pressO2 = cd.pO2;
    newInData.lungVol = cd.lvol;

    // Valve data
    newInData.isAOpen = cd.valveAopen;
    newInData.isBOpen = cd.valveBopen;
    newInData.isCOpen = cd.valveCopen;
    newInData.isDOpen = cd.valveCopen;

    emit sigNewInData(newInData);
}
