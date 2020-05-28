
#include "TreatmentCtrl.h"

namespace
{
}

TreatmentCtrl::TreatmentCtrl()
    : m_widget(new TreatmentWidget)
{
}

void TreatmentCtrl::show()
{
    m_widget->show();
}

UserInputData TreatmentCtrl::getUserInputData()
{
    return m_widget->getUserInputData();
}

void TreatmentCtrl::setUserInputData(const UserInputData& input)
{
    m_widget->setUserInputData(input);
}
