#include <QDebug>

#include "MainCtrl.h"
#include "TreatmentCtrl.h"

namespace
{
}

TreatmentCtrl::TreatmentCtrl(MainCtrl *mainCtrl)
    : m_widget(new TreatmentWidget)
    , m_mainCtrl(mainCtrl)
{
    connect(m_widget, &TreatmentWidget::sigPowerupButtonClicked, this, &TreatmentCtrl::onPowerupButtonClicked);
}

TreatmentCtrl::~TreatmentCtrl()
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

void TreatmentCtrl::onPowerupButtonClicked()
{
    qDebug() << "TreatmentCtrl::onPowerupButtonClicked called.";

    // Powerup button clicked. Show the powerup page.
    m_mainCtrl->showPage(Pages::PagePowerup);
}
