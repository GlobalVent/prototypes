#include "MainWidget.h"
#include "ui_MainWidget.h"

#include <QChartView>

#include "LabeledInputWidget.h"
#include "ChartWidget.h"
#include "RedGreenWidget.h"
#include "GraphWidget.h"

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

    // Add bottom row of input widgets
    auto inputLayout = ui->inputFrame->layout();
    inputLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    inputLayout->setSpacing(24);
    inputLayout->addWidget(new LabeledInputWidget{tr("FiO2")});
    inputLayout->addWidget(new LabeledInputWidget{tr("Total Vol.")});
    inputLayout->addWidget(new LabeledInputWidget{tr("Resp Rate")});
    inputLayout->addWidget(new LabeledInputWidget{tr("I:E")});
    inputLayout->addWidget(new LabeledInputWidget{tr("PEEP")});
    inputLayout->addWidget(new LabeledInputWidget{tr("Vent Mode")});

    // Add left column of data widgets
    //auto dataLayout = ui->dataFrame->layout();
    //dataLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    const qreal TimerInterval_ms = 100;

#if 1
    // Upper left graph
    ChartWidget::InitParams ulParams;
    ulParams.type = ChartWidget::ChartType::Scatter;
    ulParams.yAxisTitle = "Press(cmH2O)";
    ulParams.xAxisTickCount = 7;
    ulParams.xAxisMinorTickCount = 4;
    ulParams.xAxisMin = 0.0;
    ulParams.xAxisMax = 6.0;
    ulParams.yAxisTickCount = 5;
    ulParams.yAxisMin = 0.0;
    ulParams.yAxisMax = 35.0;

    m_ulGraph = new ChartWidget(ulParams);
    m_ulGraph->resize(ui->upperLeftGraphFrame->size());
    QChartView *ulChartView = new QChartView(m_ulGraph, ui->upperLeftGraphFrame);
    ulChartView->setRenderHint(QPainter::Antialiasing);
    ulChartView->resize(ui->upperLeftGraphFrame->size());
#endif

#if 1
    // Upper right graph
    GraphWidget::InitParams urParams;
    //urParams.yAxisTitle = "Flow(L/min)";
    urParams.xAxisTickCount = 60;
    urParams.xAxisMin = -1.0;
    urParams.xAxisMax = 2.0;
    urParams.yAxisMin = 0.0;
    urParams.yAxisMax = 6.0;

    m_urGraph = new GraphWidget(urParams, ui->upperRightGraphFrame);
#endif

#if 0
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
