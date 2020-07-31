#include <QDebug>

#include "InputEventMgr.h"

namespace
{
}

InputEventMgr::InputEventMgr()
{
}

void InputEventMgr::onKnobEvent(QKeyEvent* event)
{
    qDebug() << "InputEventMgr::onKnobEvent(" << event << ") called.";
    // JPW @todo fill in
}
