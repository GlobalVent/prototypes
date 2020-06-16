#ifndef POWERUPCTRL_H
#define POWERUPCTRL_H

#include <QObject>

#include "PowerupWidget.h"

class MainCtrl;

class PowerupCtrl : public QObject
{
    Q_OBJECT
  public:
    PowerupCtrl() = delete;
    PowerupCtrl(MainCtrl* mainCtrl);
    ~PowerupCtrl();

    PowerupWidget *getWidget();

  private slots:
    void onTreatmentButtonClicked();

  private:
    PowerupWidget* m_widget = nullptr;
    MainCtrl *m_mainCtrl = nullptr;
};
#endif // POWERUPCTRL_H
