#ifndef TREATMENTCTRL_H
#define TREATMENTCTRL_H

#include <QObject>

#include "UserInputData.h"
#include "TreatmentWidget.h"

class TreatmentCtrl : public QObject
{
    Q_OBJECT
public:
    TreatmentCtrl();
    ~TreatmentCtrl() = default;

    void show();

    UserInputData getUserInputData();
    void setUserInputData(const UserInputData& data);

private:
    TreatmentWidget* m_widget = nullptr;
};
#endif // TREATMENTCTRL_H
