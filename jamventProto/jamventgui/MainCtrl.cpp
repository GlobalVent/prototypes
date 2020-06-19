
#include "Pages.h"
#include "MainCtrl.h"
#include "MainWidget.h"
#include "PowerupCtrl.h"
#include "TreatmentCtrl.h"

namespace
{
    constexpr bool useSerialInterface = false;
}

MainCtrl::MainCtrl()
    : m_widget(new MainWidget)
    , m_powerupCtrl(new PowerupCtrl(this))
    , m_treatmentCtrl(new TreatmentCtrl(this))
    , m_commMgr()
    , m_serialMgr()
{
    TreatmentWidget* treatmentWidget = m_treatmentCtrl->getWidget();
    PowerupWidget *powerupWidget = m_powerupCtrl->getWidget();

    // Add all the page widgets to the main widget.
    m_widget->addWidget(Pages::PagePowerup, powerupWidget);
    m_widget->addWidget(Pages::PageTreatment, treatmentWidget);

    if (useSerialInterface)
    {
        connect(treatmentWidget, &TreatmentWidget::sigValueAOpenChanged, &m_serialMgr, &SerialMgr::onValveAOpenChanged);
        connect(treatmentWidget, &TreatmentWidget::sigValueBOpenChanged, &m_serialMgr, &SerialMgr::onValveCOpenChanged);
        connect(treatmentWidget, &TreatmentWidget::sigValueCOpenChanged, &m_serialMgr, &SerialMgr::onValveBOpenChanged);
        connect(treatmentWidget, &TreatmentWidget::sigValueDOpenChanged, &m_serialMgr, &SerialMgr::onValveDOpenChanged);
        connect(&m_serialMgr, &SerialMgr::sigNewInData, treatmentWidget, &TreatmentWidget::onNewInData);

        m_serialMgr.start();
    }
    else
    {
        // Default is use the jamvent ctrl I2C interface
        // Connections
        // Connect CommMgr to Widgets
        connect(treatmentWidget, &TreatmentWidget::sigValueAOpenChanged, &m_commMgr, &CommMgr::onValveAOpenChanged);
        connect(treatmentWidget, &TreatmentWidget::sigValueBOpenChanged, &m_commMgr, &CommMgr::onValveCOpenChanged);
        connect(treatmentWidget, &TreatmentWidget::sigValueCOpenChanged, &m_commMgr, &CommMgr::onValveBOpenChanged);
        connect(treatmentWidget, &TreatmentWidget::sigValueDOpenChanged, &m_commMgr, &CommMgr::onValveDOpenChanged);
        connect(&m_commMgr, &CommMgr::sigNewInData, treatmentWidget, &TreatmentWidget::onNewInData);

        m_commMgr.start();
    }
#if 1
    // Start by showing the Treatment Page.
    showPage(Pages::PageTreatment);
#else    // Start by showing the Powerup Page.
   showPage(Pages::PagePowerup);#endif
}

MainWidget* MainCtrl::getWidget()
{
    return m_widget;
}

void MainCtrl::showPage(Pages::Page_E page)
{
    m_widget->showPage(page);
}
