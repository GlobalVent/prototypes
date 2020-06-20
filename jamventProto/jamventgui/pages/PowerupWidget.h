#ifndef POWERUPWIDGET_H
#define POWERUPWIDGET_H

#include <QWidget>
#include <QSpinBox>

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
      void sigValueAOpenChanged(bool isOpen);
      void sigValueBOpenChanged(bool isOpen);
      void sigValueCOpenChanged(bool isOpen);
      void sigValueDOpenChanged(bool isOpen);

      void sigUseSerialChanged(bool isChecked);
      void sigSimOnChanged(bool isChecked);
      void sigTreatmentButtonClicked();

    private:
      void onValveAChanged(bool isChecked);
      void onValveBChanged(bool isChecked);
      void onValveCChanged(bool isChecked);
      void onValveDChanged(bool isChecked);

      void onUseSerialChanged(bool isChecked);
      void onSimOnChanged(bool isChecked);

    private:
      Ui::PowerupWidget *ui;

      // Value A-D button widgets
      PushButtonWidget *m_valveAButtonWidget;
      PushButtonWidget *m_valveBButtonWidget;
      PushButtonWidget *m_valveCButtonWidget;
      PushButtonWidget *m_valveDButtonWidget;

      PushButtonWidget *m_useSerialButtonWidget = nullptr;
      PushButtonWidget *m_simOnButtonWidget = nullptr;
      PushButtonWidget *m_treatmentButtonWidget = nullptr;

      // User Input Data widgets
      QSpinBox *m_maxPressSpinBox = nullptr;
      QSpinBox *m_vaTargetSpinBox = nullptr;
      QSpinBox *m_vbTargetSpinBox = nullptr;
      QSpinBox *m_sysTargetSpinBox = nullptr;

      // Value A-D button checked state
      bool m_isValueAChecked = false;
      bool m_isValueBChecked = false;
      bool m_isValueCChecked = false;
      bool m_isValueDChecked = false;

      bool m_isUseSerialChecked = false;
      bool m_isSimOnChecked = false;
};
#endif // POWERUPWIDGET_H
