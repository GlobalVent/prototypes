
#include "TreatmentCtrl.h"

namespace
{
}

TreatmentCtrl::TreatmentCtrl()
    : m_widget(new TreatmentWidget)
{
}

TreatmentWidget* TreatmentCtrl::getWidget()
{
    return m_widget;
}

UserInputData TreatmentCtrl::getUserInputData()
{
    return m_widget->getUserInputData();
}

void TreatmentCtrl::setUserInputData(const UserInputData& input)
{
    m_widget->setUserInputData(input);
}
