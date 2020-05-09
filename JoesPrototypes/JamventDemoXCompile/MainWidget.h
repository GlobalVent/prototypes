#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QtCore/QTimer>

#include "ChartWidget.h"
#include "RedGreenWidget.h"
#include "GraphWidget.h"

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
  float getSinValue(int tick, int tickCount);

  Ui::MainWidget *ui;

  GraphWidget::InitParams m_ulParams;
  GraphWidget *m_ulGraph = nullptr;

  GraphWidget::InitParams m_urParams;
  GraphWidget *m_urGraph = nullptr;

  ChartWidget *m_llGraph = nullptr;
  RedGreenWidget *m_lrGraph = nullptr;

  int m_timerInterval_ms = 0;
  QTimer m_timer;
};
#endif // MAINWIDGET_H
