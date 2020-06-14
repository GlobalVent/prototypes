
#include "CommMgr.h"

namespace
{
}

CommMgr::CommMgr()
    : QObject()
{
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
}

void CommMgr::onValveBOpenChanged(bool isOpen)
{
}

void CommMgr::onValveCOpenChanged(bool isOpen)
{
}

void CommMgr::onValveDOpenChanged(bool isOpen)
{
}
