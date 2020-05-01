#include "MainWidget.h"
#include "ui_MainWidget.h"

#include <QChartView>

#include "ChartWidget.h"
#include "RedGreenWidget.h"

using namespace QtCharts;

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , m_ulGraph(nullptr)
    , m_urGraph(nullptr)
    , m_llGraph(nullptr)
    , m_lrGraph(nullptr)
    , m_timerInterval_ms(0)
    , m_timer()
{
    ui->setupUi(this);

    const qreal TimerInterval_ms = 100; //250;

#if 0
    // Upper left graph
    ChartWidget::InitParams ulParams;
    ulParams.type = ChartWidget::ChartType::Scatter;
    ulParams.yAxisTitle = "Press(cmH2O)";
	ulParams.timerInterval_ms = TimerInterval_ms;
    ulParams.xAxisTickCount = 7;
    ulParams.xAxisMinorTickCount = 4;
    ulParams.xAxisMin = 0.0;
    ulParams.xAxisMax = 6.0;
    ulParams.yAxisTickCount = 5;
    ulParams.yAxisMin = 0.0;
    ulParams.yAxisMax = 35.0;

    m_ulGraph = new ChartWidget(ulParams);
    m_ulChart->resize(ui->upperLeftGraphFrame->size());
    QChartView *ulChartView = new QChartView(m_ulChart, ui->upperLeftGraphFrame);
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
    urParams.xAxisMinorTickCount = 4;
    urParams.xAxisMin = 0.0;
    urParams.xAxisMax = 6.0;
    urParams.yAxisTickCount = 5;
    urParams.yAxisMin = -20.0;
    urParams.yAxisMax = 40.0;

    m_urGraph = new ChartWidget(urParams);
    m_urChart->resize(ui->upperRightGraphFrame->size());
    QChartView *urChartView = new QChartView(m_urChart, ui->upperRightGraphFrame);
    urChartView->setRenderHint(QPainter::Antialiasing);
    urChartView->resize(ui->upperRightGraphFrame->size());
#endif

#if 1
    // Lower left graph
    ChartWidget::InitParams llParams;
    llParams.type = ChartWidget::ChartType::Line;
    llParams.yAxisTitle = "";
    llParams.xAxisTickCount = 7;
    llParams.xAxisMinorTickCount = 10;
    llParams.xAxisMin = 0.0;
    llParams.xAxisMax = 6.0;
    llParams.yAxisTickCount = 5;
    llParams.yAxisMin = 0.0;
    llParams.yAxisMax = 30.0;

    m_llGraph = new ChartWidget(llParams);
    m_llGraph->resize(ui->lowerLeftGraphFrame->size());
    QChartView *llChartView = new QChartView(m_llGraph, ui->lowerLeftGraphFrame);
    llChartView->setRenderHint(QPainter::Antialiasing);
    llChartView->resize(ui->lowerLeftGraphFrame->size());
#endif

#if 1
    // Lower right graph
    RedGreenWidget::InitParams lrParams;
    lrParams.xAxisTickCount = 60;
    m_lrGraph = new RedGreenWidget(lrParams, ui->lowerRightGraphFrame);
#endif

     QObject::connect(&m_timer, &QTimer::timeout, this, &MainWidget::onTimeout);
     m_timer.setInterval(TimerInterval_ms);
     m_timer.start();
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

void MainWidget::onTimeout()
{

    if (nullptr != m_ulGraph)
    {
        m_ulGraph->onTimeout();
    }

    if (nullptr != m_urGraph)
    {
        m_urGraph->onTimeout();
    }

    if (nullptr != m_llGraph)
    {
        m_llGraph->onTimeout();
    }

    if (nullptr != m_lrGraph)
    {
        m_lrGraph->onTimeout();
    }
}