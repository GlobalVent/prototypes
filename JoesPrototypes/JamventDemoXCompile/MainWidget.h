#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QtCore/QTimer>

#include "ChartWidget.h"
#include "RedGreenWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void keyPressEvent(QKeyEvent *event) override;
    void onTimeout();

private:
    Ui::MainWidget *ui;

    ChartWidget *m_ulGraph = nullptr;
    ChartWidget *m_urGraph = nullptr;
    ChartWidget *m_llGraph = nullptr;
    RedGreenWidget *m_lrGraph = nullptr;

    int m_timerInterval_ms = 0;
    QTimer m_timer;
};
#endif // MAINWIDGET_H
