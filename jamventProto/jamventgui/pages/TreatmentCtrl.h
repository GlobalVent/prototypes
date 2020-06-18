#ifndef TREATMENTCTRL_H
#define TREATMENTCTRL_H

#include <QObject>

#include "UserInputData.h"
#include "TreatmentWidget.h"

class MainCtrl;

class TreatmentCtrl : public QObject
{
    Q_OBJECT
public:
    TreatmentCtrl() = delete;
    TreatmentCtrl(MainCtrl *mainCtrl);
    ~TreatmentCtrl();

    TreatmentWidget* getWidget();

    UserInputData getUserInputData();
    void setUserInputData(const UserInputData& data);

  private slots:
    void onPowerupButtonClicked();

  private:
    TreatmentWidget* m_widget = nullptr;
    MainCtrl *m_mainCtrl = nullptr;
};
#endif // TREATMENTCTRL_H
