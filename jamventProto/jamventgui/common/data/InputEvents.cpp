#include <QDebug>

#include "InputEvents.h"

namespace
{
}

// Button Clicked event as a "c" key press event.
QKeyEvent *InputEvents::ButtonClickedEventCreate()
{
    return new QKeyEvent{QKeyEvent::KeyPress, Qt::Key_C, Qt::NoModifier, "c"};
}

// Button pressed event as a "d" key press event.
QKeyEvent *InputEvents::ButtonPressedEventCreate()
{
    return new QKeyEvent{QKeyEvent::KeyPress, Qt::Key_D, Qt::NoModifier, "d"};
}

// Button released event as a "u" key press event.
QKeyEvent *InputEvents::ButtonReleasedEventCreate()
{
    return new QKeyEvent{QKeyEvent::KeyPress, Qt::Key_U, Qt::NoModifier, "u"};
}

// Button long press event as a "p" key press event.
QKeyEvent *InputEvents::ButtonLongPressEventCreate()
{
    return new QKeyEvent{QKeyEvent::KeyPress, Qt::Key_P, Qt::NoModifier, "p"};
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
