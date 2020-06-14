
#include "Pages.h"
#include "MainCtrl.h"
#include "MainWidget.h"
#include "TreatmentCtrl.h"

namespace
{
}

MainCtrl::MainCtrl()
    : m_widget(new MainWidget)
    , m_treatmentCtrl(new TreatmentCtrl)
    , m_commMgr()
{
    TreatmentWidget* treatmentWidget = m_treatmentCtrl->getWidget();

    // Add all the page widgets to the main widget.
    m_widget->addWidget(Pages::PageTreatment, treatmentWidget);

    // Connections
    // Connect CommMgr to Widgets
    connect(treatmentWidget, &TreatmentWidget::sigValueAOpenChanged, &m_commMgr, &CommMgr::onValveAOpenChanged);
    connect(treatmentWidget, &TreatmentWidget::sigValueBOpenChanged, &m_commMgr, &CommMgr::onValveCOpenChanged);
    connect(treatmentWidget, &TreatmentWidget::sigValueCOpenChanged, &m_commMgr, &CommMgr::onValveBOpenChanged);
    connect(treatmentWidget, &TreatmentWidget::sigValueDOpenChanged, &m_commMgr, &CommMgr::onValveDOpenChanged);

    connect(&m_commMgr, &CommMgr::sigNewInData, treatmentWidget, &TreatmentWidget::onNewInData);

    // Start by showing the Treatment Page
    showPage(Pages::PageTreatment);
}

MainWidget* MainCtrl::getWidget()
{
    return m_widget;
}

void MainCtrl::showPage(Pages::Page_E page)
{
    m_widget->showPage(page);
}
