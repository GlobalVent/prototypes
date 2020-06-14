#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QHash>
#include <QWidget>
#include <QStackedWidget>

#include "JamCtrlMgr.h"
#include "Pages.h"
#include "MainCtrl.h"
#include "TreatmentWidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void setJamCtlMgr(JamCtrlMgr *jamCtrlMgr) { m_treatmentWidget->setJamCtrlMgr(jamCtrlMgr); };

    void addWidget(Pages::Page_E page, QWidget *widget);
    void showPage(Pages::Page_E page);

private:
    void keyPressEvent(QKeyEvent *event) override;

    using PageMap = QHash<Pages::Page_E, QWidget *>;
    PageMap m_pageMap;

    QStackedWidget* m_stackedWidget;
    TreatmentWidget* m_treatmentWidget;
};
#endif // MAINWIDGET_H
