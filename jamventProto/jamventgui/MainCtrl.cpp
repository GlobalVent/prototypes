
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

    // JPW @todo. For now share controller with widget.
    treatmentWidget->setJamCtrlMgr(m_commMgr.getJamCtrlMgr());

    // Add all the page widgets to the main widget.
    m_widget->addWidget(Pages::PageTreatment, treatmentWidget);

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
