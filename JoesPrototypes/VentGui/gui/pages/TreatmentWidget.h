#ifndef TREATMENTWIDGET_H
#define TREATMENTWIDGET_H

#include <QWidget>
#include <QSpinBox>
#include <QKeyEvent>
#include <QtCore/QTimer>

#include "UserInputData.h"
#include "PushButtonWidget.h"
#include "GraphWidget.h"
#include "SerialMgr.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class TreatmentWidget;
}
QT_END_NAMESPACE

class TreatmentWidget : public QWidget
{
    Q_OBJECT

public:
    TreatmentWidget(QWidget *parent = nullptr);
    ~TreatmentWidget();

    UserInputData getUserInputData();
    void setUserInputData(const UserInputData& data);

    void keyPressEvent(QKeyEvent *event) override;
    void onTimeout();

private:
  float getSinValue(int tick, int tickCount);

  void onValveAToggled(bool isChecked);
  void onValveBToggled(bool isChecked);
  void onValveCToggled(bool isChecked);
  void onValveDToggled(bool isChecked);

  Ui::TreatmentWidget *ui;

  GraphWidget::InitParams m_ulParams;
  GraphWidget *m_ulGraph = nullptr;

  GraphWidget::InitParams m_urParams;
  GraphWidget *m_urGraph = nullptr;

  GraphWidget::InitParams m_llParams;
  GraphWidget *m_llGraph = nullptr;

  GraphWidget::InitParams m_lrParams;
  GraphWidget *m_lrGraph = nullptr;

  // User Input Data widgets
  QSpinBox* m_fiO2SpinBox = nullptr;
  QSpinBox *m_tidalVolSpinBox = nullptr;
  QSpinBox *m_respRateSpinBox = nullptr;
  QSpinBox *m_ieSpinBox = nullptr;
  QSpinBox *m_peepSpinBox = nullptr;
  // Vent Mode?

  // Value A-D button widgets
  PushButtonWidget* m_valveAButtonWidget;
  PushButtonWidget* m_valveBButtonWidget;
  PushButtonWidget* m_valveCButtonWidget;
  PushButtonWidget* m_valveDButtonWidget;

  // Value A-D button checked state
  bool m_isValueAChecked = false;
  bool m_isValueBChecked = false;
  bool m_isValueCChecked = false;
  bool m_isValueDChecked = false;

  // JPW @todo temporary here for convenience
  SerialMgr m_serialMgr;

  int m_timerInterval_ms = 0;
  QTimer m_timer;
};
#endif // TREATMENTWIDGET_H
