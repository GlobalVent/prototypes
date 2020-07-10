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
}

PushButtonWidget::PushButtonWidget(const QString& label, QWidget* parent)
    : QPushButton(label, parent)
{
    setStyleSheet(ButtonStyle);
}

bool PushButtonWidget::event(QEvent *event)
{
    //qDebug() << "event(" << event << ") called.";

    QKeyEvent *ke = static_cast<QKeyEvent *>(event);

    if ((event->type() == QEvent::KeyPress) &&
        ((ke->key() == Qt::Key_Up) || (ke->key() == Qt::Key_Down)))
    {
        // Treat up/down arrows as a click.
        click();
        // Handle up/down arrow key press manually.
        return true;
    }

    return QWidget::event(event);
}
