#ifndef POWERUPWIDGET_H
#define POWERUPWIDGET_H

#include <QWidget>

#include "PushButtonWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class PowerupWidget;
}
QT_END_NAMESPACE

class PowerupWidget : public QWidget
{
    Q_OBJECT

public:
    PowerupWidget(QWidget *parent = nullptr);
    ~PowerupWidget();

    PowerupWidget *getWidget();

    signals:
      void sigTreatmentButtonClicked();

    private:
      Ui::PowerupWidget *ui;
      PushButtonWidget *m_treatmentButtonWidget;
};
#endif // POWERUPWIDGET_H
