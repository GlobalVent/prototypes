#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QStackedWidget>

#include "TreatmentWidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    QStackedWidget* m_stackedWidget;

    TreatmentWidget* m_treatmentWidget;
};
#endif // MAINWIDGET_H
