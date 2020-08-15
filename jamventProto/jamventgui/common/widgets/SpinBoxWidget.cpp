#include <QDebug>

//#include "Theme.h"
#include "SpinBoxWidget.h"

namespace
{
}

SpinBoxWidget ::SpinBoxWidget(QWidget *parent)
    : QSpinBox(parent)
{
}

void SpinBoxWidget::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "SpinBoxWidget::keyReleaseEvent(" << event << ") called.";

    if (event->key() == Qt::Key_R)
    {
        // Clockwise click. Increment.
        stepUp();
    }
    else if (event->key() == Qt::Key_L)
    {
        // Counter clockwise click. Decrement.
        stepDown();
    }
    else if (event->key() == Qt::Key_C)
    {
        // Button pressed
        // Ignore for now.
        QWidget::keyPressEvent(event);
    }
    else
    {
        QWidget::keyPressEvent(event);
    }
}

void SpinBoxWidget::focusInEvent(QFocusEvent *event)
{
    qDebug() << "SpinBoxWidget::focusInEvent(" << event << ") called.";
    emit sigFocusChanged(true);
}

void SpinBoxWidget::focusOutEvent(QFocusEvent *event)
{
    qDebug() << "SpinBoxWidget::focusOutEvent(" << event << ") called.";
    emit sigFocusChanged(false);
}
