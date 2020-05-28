
#include "MainCtrl.h"
#include "TreatmentCtrl.h"

namespace
{
}

MainCtrl::MainCtrl()
    : m_treatmentCtrl(new TreatmentCtrl)
{
}


void MainCtrl::show(Page_E page)
{
    // \todo Generalize to show any page
    m_treatmentCtrl->show();
}
