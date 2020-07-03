#ifndef MAINCTRL_H
#define MAINCTRL_H

#include <QObject>

#include "Pages.h"
#include "CommMgr.h"
#include "PowerupCtrl.h"
#include "TreatmentCtrl.h"
#include "SerialMgr.h"

class MainWidget;

class MainCtrl : public QObject
{
    Q_OBJECT

public:
    MainCtrl();
    ~MainCtrl() = default;

    MainWidget* getWidget();
    void showPage(Pages::Page_E page);

  public slots:
    void onUserSerialChanged(bool isChecked);
    void onSimOnChanged(bool isChecked);

  private:
    CommMgr m_commMgr;      // For now this is connected to jamCtrl
    SerialMgr m_serialMgr;

    MainWidget* m_widget = nullptr;

    // Controllers for each page
    PowerupCtrl *m_powerupCtrl = nullptr;
    TreatmentCtrl*  m_treatmentCtrl = nullptr;
};
#endif // MAINCTRL_H
