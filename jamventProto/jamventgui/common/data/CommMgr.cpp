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
    JamCtrlSim::DataIn cd;

    cd = m_jamCtrl.getCtrlData();

    qDebug() << "CommMgr::onTimeout(): pSys_bar = " << cd.pSys_bar << ", pRes_bar = " << cd.pRes_bar  << "o2_pc = " << cd.o2_pc << ", lungVol_ml = " << cd.lungVol_ml;

    // NOTE: scaling here is really artificial as these are not the data
    //       we are ultimatly graphing, but the data that we are just
    //       using to do a milestone.

    // JPW: Above note from Ralph.  New code below just passes on the data from the jamCtrl manager. Scaling happens when
    // data is applied to the graphs.

    JamCtrlSim::DataIn newInData;

    // Graph data
    // JPW @todo data massaging is to match the simulator. Remove/change when no longer necessary.
    newInData.pSys_bar = cd.pSys_bar / 3.0;  // Massage to match simulator.
    newInData.pRes_bar = (cd.pRes_bar / 1000) - 1.0; // Massage to match simulator.
    newInData.o2_pc = cd.o2_pc;
    newInData.lungVol_ml = cd.lungVol_ml;

    // Valve data
    newInData.isAOpen = cd.isAOpen;
    newInData.isBOpen = cd.isBOpen;
    newInData.isCOpen = cd.isCOpen;
    newInData.isDOpen = cd.isCOpen;

    emit sigNewInData(newInData);
}
