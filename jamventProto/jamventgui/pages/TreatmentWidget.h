#ifndef TREATMENTWIDGET_H
#define TREATMENTWIDGET_H

#include <QWidget>
#include <QSpinBox>

#include "UserInputData.h"
#include "PushButtonWidget.h"
#include "GraphWidget.h"
#include "CommMgr.h"

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

  public slots:
    void onNewInData(CommMgr::DataIn data);

signals:
  void sigFio2Changed(int value);
  void sigTidalVolChanged(int value);
  void sigRespRateChanged(int value);
  void sigIeRatioChanged(int value);
  void sigPeepChanged(int value);

  void sigPowerupButtonClicked();

private:
  void onFio2Changed(int value);
  void onTidalVolChanged(int value);
  void onRespRateChanged(int value);
  void onIeRatioChanged(int value);
  void onPeepChanged(int value);

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
  SpinBoxWidget *m_fio2SpinBox = nullptr;
  SpinBoxWidget *m_tidalVolSpinBox = nullptr;
  SpinBoxWidget *m_respRateSpinBox = nullptr;
  SpinBoxWidget *m_ieRatioSpinBox = nullptr;
  SpinBoxWidget *m_peepSpinBox = nullptr;
  // JPW @todo Add Vent Mode when understand and used.

  PushButtonWidget *m_powerupButtonWidget = nullptr;
};
#endif // TREATMENTWIDGET_H
