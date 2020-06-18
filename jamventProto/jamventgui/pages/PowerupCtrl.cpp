#include <QDebug>

#include "MainCtrl.h"
#include "PowerupCtrl.h"

namespace
{
}

PowerupCtrl::PowerupCtrl(MainCtrl *mainCtrl)
    : m_widget(new PowerupWidget)
    , m_mainCtrl(mainCtrl)
{
    connect(m_widget, &PowerupWidget::sigTreatmentButtonClicked, this, &PowerupCtrl::onTreatmentButtonClicked);
}

PowerupCtrl::~PowerupCtrl()
{
}

PowerupWidget *PowerupCtrl::getWidget()
{
    return m_widget;
}

void PowerupCtrl::onTreatmentButtonClicked()
{
    qDebug() << "PowerupCtrl::onTreatmentButtonClicked called.";

    // Treatment button clicked. Show the treatment page.
    m_mainCtrl->showPage(Pages::PageTreatment);
}
