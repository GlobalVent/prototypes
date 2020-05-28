
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
