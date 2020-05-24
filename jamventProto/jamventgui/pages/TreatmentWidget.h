#ifndef TREATMENTWIDGET_H
#define TREATMENTWIDGET_H

#include <QWidget>
#include <QSpinBox>
#include <QKeyEvent>
#include <QtCore/QTimer>

#include "UserInputData.h"
#include "GraphWidget.h"

#include "JamCtrlMgr.h"

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


    void setJamCtrlMgr(JamCtrlMgr *jamCtrlMgr) { m_jamCtrlMgr=jamCtrlMgr;};

private:
  float getSinValue(int tick, int tickCount);

  Ui::TreatmentWidget *ui;

  GraphWidget::InitParams m_ulParams;
  GraphWidget *m_ulGraph = nullptr;

  GraphWidget::InitParams m_urParams;
  GraphWidget *m_urGraph = nullptr;

  GraphWidget *m_llGraph = nullptr;
  GraphWidget *m_lrGraph = nullptr;

  // User Input Data widgets
  QSpinBox* m_fiO2SpinBox = nullptr;
  QSpinBox *m_tidalVolSpinBox = nullptr;
  QSpinBox *m_respRateSpinBox = nullptr;
  QSpinBox *m_ieSpinBox = nullptr;
  QSpinBox *m_peepSpinBox = nullptr;
  // Vent Mode?

  int m_timerInterval_ms = 0;
  QTimer m_timer;

  JamCtrlMgr *m_jamCtrlMgr;
};
#endif // TREATMENTWIDGET_H
