#include <QDebug>

#include "Pages.h"
#include "MainCtrl.h"
#include "MainWidget.h"
#include "PowerupCtrl.h"
#include "TreatmentCtrl.h"

namespace
{
   
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

    // Connect powerup page to main controller
    connect(powerupWidget, &PowerupWidget::sigUseSerialChanged, this, &MainCtrl::onUserSerialChanged);
    connect(powerupWidget, &PowerupWidget::sigSimOnChanged, this, &MainCtrl::onSimOnChanged);

    // Connect treatment page to serial communication manager.
    connect(treatmentWidget, &TreatmentWidget::sigFio2Changed, &m_serialMgr, &SerialMgr::onFio2Changed);
    connect(treatmentWidget, &TreatmentWidget::sigTidalVolChanged, &m_serialMgr, &SerialMgr::onTidalVolChanged);
    connect(treatmentWidget, &TreatmentWidget::sigRespRateChanged, &m_serialMgr, &SerialMgr::onRespRateChanged);
    connect(treatmentWidget, &TreatmentWidget::sigIeRatioChanged, &m_serialMgr, &SerialMgr::onIeRatioChanged);
    connect(treatmentWidget, &TreatmentWidget::sigPeepChanged, &m_serialMgr, &SerialMgr::onPeepChanged);
    connect(&m_serialMgr, &SerialMgr::sigNewInData, treatmentWidget, &TreatmentWidget::onNewInData);

    // Connect the powerup page to the serial communicaiton manager.
    connect(powerupWidget, &PowerupWidget::sigValueAOpenChanged, &m_serialMgr, &SerialMgr::onValveAOpenChanged);
    connect(powerupWidget, &PowerupWidget::sigValueBOpenChanged, &m_serialMgr, &SerialMgr::onValveCOpenChanged);
    connect(powerupWidget, &PowerupWidget::sigValueCOpenChanged, &m_serialMgr, &SerialMgr::onValveBOpenChanged);
    connect(powerupWidget, &PowerupWidget::sigValueDOpenChanged, &m_serialMgr, &SerialMgr::onValveDOpenChanged);
    // Temporary
    connect(powerupWidget, &PowerupWidget::sigMaxPressChanged, &m_serialMgr, &SerialMgr::onMaxPressChanged);
    connect(powerupWidget, &PowerupWidget::sigVaTargetChanged, &m_serialMgr, &SerialMgr::onVaTargetChanged);
    connect(powerupWidget, &PowerupWidget::sigVbTargetChanged, &m_serialMgr, &SerialMgr::onVbTargetChanged);
    connect(powerupWidget, &PowerupWidget::sigSysTargetChanged, &m_serialMgr, &SerialMgr::onSysTargetChanged);

    // Connect treatment page to I2C communications manager.
    connect(&m_commMgr, &CommMgr::sigNewInData, treatmentWidget, &TreatmentWidget::onNewInData);

    // Connect the powerup page to the I2C communications manager.
    connect(powerupWidget, &PowerupWidget::sigValueAOpenChanged, &m_commMgr, &CommMgr::onValveAOpenChanged);
    connect(powerupWidget, &PowerupWidget::sigValueBOpenChanged, &m_commMgr, &CommMgr::onValveCOpenChanged);
    connect(powerupWidget, &PowerupWidget::sigValueCOpenChanged, &m_commMgr, &CommMgr::onValveBOpenChanged);
    connect(powerupWidget, &PowerupWidget::sigValueDOpenChanged, &m_commMgr, &CommMgr::onValveDOpenChanged);

    if (m_useSerialInterface)
    {
        m_serialMgr.start();
    }
    else
    {
        // Default is use the jamvent ctrl I2C interface
        m_commMgr.start();
    }

#if 0
    // Start by showing the Treatment Page.
    showPage(Pages::PageTreatment);
#else
    // Start by showing the Powerup Page.
   showPage(Pages::PagePowerup);
#endif
}

MainWidget* MainCtrl::getWidget()
{
    return m_widget;
}

void MainCtrl::showPage(Pages::Page_E page)
{
    m_widget->showPage(page);
}

void MainCtrl::onUserSerialChanged(bool isChecked)
{
    qDebug() << "MainCtrl::onUserSerialChanged(" << isChecked << ")";
    if (isChecked)
    {
        // Stop I2C. Start serial.
        m_commMgr.stop();
        m_serialMgr.start();
    }
    else
    {
        // Stop serial. Start I2C.
        m_serialMgr.stop();
        m_commMgr.start();
    }
}

void MainCtrl::onSimOnChanged(bool isChecked)
{
    qDebug() << "MainCtrl::onSimOnChanged(" << isChecked << ")";

    if (isChecked)
    {
        // Simulation started. Send start commands (differs for serail vs I2C)
    }
    else
    {
        // Simulation stopped. Send stop commands (if any. differs for serail vs I2C)
    }
}
