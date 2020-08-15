#include <QDebug>
#include <QKeyEvent>

#include "Theme.h"
#include "PushButtonWidget.h"

namespace
{
    const QString ButtonStyle =
        "QPushButton {"
        "  font-family: 'Roboto';"
        "  font-size: 12px;"
        "  font-weight: 400;"
        "  color: black;"
        "  border: 2px solid gray;"
        "  border-radius: 8px;"
        "  min-width: 100px;"
        "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
        "                              stop: 0 #f6f7fa, stop: 1 #dadbde);"
        "}"
        "QPushButton:pressed {"
        "  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
        "                                    stop: 0 #dadbde, stop: 1 #f6f7fa);"
        "}" 
        "QPushButton:checked {"
        "  background-color : gray;"
        "}";

    const QString NoFocusStyle = ".PushButtonWidget { border: 2px solid gray; padding: 2px;}";
    const QString FocusStyle = ".PushButtonWidget { border: 2px dashed seagreen; padding: 2px; }";
    const QString FocusAndSelectedStyle = ".PushButtonWidget { border: 3px solid darkgreen; padding: 2px; }";
}

PushButtonWidget::PushButtonWidget(const QString& label, QWidget* parent)
    : QPushButton(label, parent)
{
    setStyleSheet(ButtonStyle);
}

void PushButtonWidget::setFocusState(FocusState state)
{
    qDebug() << "PushButtonWidget::setFocusState(" << static_cast <int>(state) << ") called.";

    FocusableWidget::setFocusState(state);

    if (FocusState::Focus == state)
    {
        setStyleSheet(ButtonStyle + FocusStyle);
    }
    else if (FocusState::FocusAndSelected == state)
    {
        setStyleSheet(ButtonStyle + FocusAndSelectedStyle);
    }
    else
    {
        setStyleSheet(ButtonStyle + NoFocusStyle);
    }
}

bool PushButtonWidget::event(QEvent *event)
{
    //qDebug() << "event(" << event << ") called.";

    QKeyEvent *ke = static_cast<QKeyEvent *>(event);

    // Accepts up or down arrows and "r" "l" keys to activate (clicked) the button.
    if ((event->type() == QEvent::KeyPress) &&
        ((ke->key() == Qt::Key_Up) || (ke->key() == Qt::Key_Down) || 
         (ke->key() == Qt::Key_R) || (ke->key() == Qt::Key_L)))
    {
        // Treat as a click (clicked).
        click();
        // Return true as the event was handled/accepted 
        return true;
    }

    return QWidget::event(event);
}

void PushButtonWidget::focusInEvent(QFocusEvent *event)
{
    qDebug() << "PushButtonWidget::focusInEvent(" << event << ") called.";
    setFocusState(FocusState::Focus);
}

void PushButtonWidget::focusOutEvent(QFocusEvent *event)
{
    qDebug() << "PushButtonWidget::focusOutEvent(" << event << ") called.";
    setFocusState(FocusState::NoFocus);
}
