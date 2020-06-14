#ifndef MAINCTRL_H
#define MAINCTRL_H

#include <QObject>

#include "Pages.h"
#include "CommMgr.h"
#include "TreatmentCtrl.h"

class MainWidget;

class MainCtrl : public QObject
{
    Q_OBJECT

public:
    MainCtrl();
    ~MainCtrl() = default;

    MainWidget* getWidget();
    void showPage(Pages::Page_E page);

private:
    MainWidget* m_widget = nullptr;

    // Controllers for each page
    TreatmentCtrl*  m_treatmentCtrl = nullptr;

    CommMgr m_commMgr;
};
#endif // MAINCTRL_H
