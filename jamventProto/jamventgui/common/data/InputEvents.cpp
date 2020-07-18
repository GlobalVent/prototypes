#include <QDebug>

#include "InputEvents.h"

namespace
{
}

// Clicked event as a "c" key press event.
QKeyEvent *InputEvents::ClickedEventCreate()
{
    return new QKeyEvent{QKeyEvent::KeyPress, Qt::Key_C, Qt::NoModifier, "c"};
}

// Clockwise click event as a "r" key press event.
QKeyEvent *InputEvents::CwClickEventCreate()
{
    return new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_R, Qt::NoModifier, "r");
}

// Counter clockwise click event as a "l" key press event.
QKeyEvent *InputEvents::CcwClickEventCreate()
{
    return new QKeyEvent(QKeyEvent::KeyPress, Qt::Key_L, Qt::NoModifier, "l");
}
