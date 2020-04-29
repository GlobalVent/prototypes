#include "MainWidget.h"
#include "ui_MainWidget.h"

#include <QChartView>

#include "ChartWidget.h"
#include "RedGreenWidget.h"

using namespace QtCharts;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

	const qreal TimerInterval_ms = 250;
	
#if 0
    // Upper left graph
    ChartWidget::InitParams ulParams;
    ulParams.type = ChartWidget::ChartType::Scatter;
    ulParams.yAxisTitle = "Press(cmH2O)";
	ulParams.timerInterval_ms = TimerInterval_ms;
    ulParams.xAxisTickCount = 7;
    ulParams.xAxisMin = 0.0;
    ulParams.xAxisMax = 6.0;
    ulParams.yAxisTickCount = 5;
    ulParams.yAxisMin = 0.0;
    ulParams.yAxisMax = 35.0;

    ChartWidget *ulChart = new ChartWidget(ulParams);
    ulChart->resize(ui->upperLeftGraphFrame->size());
    QChartView *ulChartView = new QChartView(ulChart, ui->upperLeftGraphFrame);
    ulChartView->setRenderHint(QPainter::Antialiasing);
    ulChartView->resize(ui->upperLeftGraphFrame->size());
#endif

#if 0
    // Upper right graph
    ChartWidget::InitParams urParams;
    urParams.type = ChartWidget::ChartType::Spline;
    urParams.yAxisTitle = "Flow(L/min)";
	urParams.timerInterval_ms = TimerInterval_ms;
    urParams.xAxisTickCount = 7;
    urParams.xAxisMin = 0.0;
    urParams.xAxisMax = 6.0;
    urParams.yAxisTickCount = 5;
    urParams.yAxisMin = -20.0;
    urParams.yAxisMax = 40.0;

    ChartWidget *urChart = new ChartWidget(urParams);
    urChart->resize(ui->upperRightGraphFrame->size());
    QChartView *urChartView = new QChartView(urChart, ui->upperRightGraphFrame);
    urChartView->setRenderHint(QPainter::Antialiasing);
    urChartView->resize(ui->upperRightGraphFrame->size());
#endif

#if 1
    // Lower left graph
    ChartWidget::InitParams llParams;
    llParams.type = ChartWidget::ChartType::Line;
    llParams.yAxisTitle = "";
	llParams.timerInterval_ms = TimerInterval_ms;
    llParams.xAxisTickCount = 7;
    llParams.xAxisMin = 0.0;
    llParams.xAxisMax = 6.0;
    llParams.yAxisTickCount = 5;
    llParams.yAxisMin = 0.0;
    llParams.yAxisMax = 30.0;

    ChartWidget *llChart = new ChartWidget(llParams);
    llChart->resize(ui->lowerLeftGraphFrame->size());
    QChartView *llChartView = new QChartView(llChart, ui->lowerLeftGraphFrame);
    llChartView->setRenderHint(QPainter::Antialiasing);
    llChartView->resize(ui->lowerLeftGraphFrame->size());
#endif

#if 1
    // Lower right graph
     RedGreenWidget* lrWidget = new RedGreenWidget(ui->lowerRightGraphFrame);
#endif
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        QApplication::instance()->quit();
    }
    else
    {
        QWidget::keyPressEvent(event);
    }
}

