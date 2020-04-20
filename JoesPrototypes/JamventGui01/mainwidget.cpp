#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

using namespace QtCharts;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    QLineSeries *series = new QLineSeries();

    series->append(0, 6);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);
    *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Pressure (last n seconds)");
    chart->setMargins(QMargins(0,0,0,0));
    chart->setBackgroundRoundness(0);
    chart->resize(ui->pressureGraphFrame->size());
    chart->setBackgroundVisible(false);

    QChartView *chartView = new QChartView(chart, ui->pressureGraphFrame);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(ui->pressureGraphFrame->size());  // Fill the pressure frame.
}

MainWidget::~MainWidget()
{
    delete ui;
}

