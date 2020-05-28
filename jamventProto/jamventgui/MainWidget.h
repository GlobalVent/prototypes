#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QStackedWidget>

#include "JamCtrlMgr.h"
#include "TreatmentWidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void setJamCtlMgr(JamCtrlMgr *jamCtrlMgr) { m_treatmentWidget->setJamCtrlMgr(jamCtrlMgr);};
private:
    QStackedWidget* m_stackedWidget;

    TreatmentWidget* m_treatmentWidget;



};
#endif // MAINWIDGET_H
