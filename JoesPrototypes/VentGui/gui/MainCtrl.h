#ifndef MAINCTRL_H
#define MAINCTL_H

#include <QObject>

//#include "MainWidget.h"

#include "TreatmentCtrl.h"

class MainCtrl : public QObject
{
    Q_OBJECT

public:
    MainCtrl();
    ~MainCtrl() = default;

    enum class Page_E
    {
        None,
        Treatment
    };

    void show(Page_E page);

private:
    // Controllers for each page
    TreatmentCtrl*  m_treatmentCtrl = nullptr;
};
#endif // MAINCTRL_H
