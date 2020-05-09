#include <QVBoxLayout>
#include <QChartView>
#include <QPushButton>
#include <QtMath>

#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "LabeledInputWidget.h"
#include "LabeledDataWidget.h"
#include "ChartWidget.h"
#include "RedGreenWidget.h"
#include "GraphWidget.h"
#include "PushButtonWidget.h"

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
    auto inputGroupFrame = ui->inputGroupFrame;
    auto inputGroupLayout = new QHBoxLayout{};
    inputGroupFrame->setLayout(inputGroupLayout);
    inputGroupLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    inputGroupLayout->setSpacing(24);
    inputGroupLayout->addWidget(new LabeledInputWidget{tr("FiO2")});
    inputGroupLayout->addWidget(new LabeledInputWidget{tr("Total Vol.")});
    inputGroupLayout->addWidget(new LabeledInputWidget{tr("Resp Rate")});
    inputGroupLayout->addWidget(new LabeledInputWidget{tr("I:E")});
    inputGroupLayout->addWidget(new LabeledInputWidget{tr("PEEP")});
    inputGroupLayout->addWidget(new LabeledInputWidget{tr("Vent Mode")});
    //inputGroupLayout->addStretch();

    // Add right column data display widgets
    //auto dataLayout = ui->dataFrame->layout();
    //dataLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    //auto dataFrame = ui->rightColumnFrame;
    auto dataGroupWidget = ui->dataGroupWidget;
    auto dataGroupLayout = new QVBoxLayout{};
    dataGroupWidget->setLayout(dataGroupLayout);
    dataGroupLayout->setAlignment(Qt::AlignTop| Qt::AlignHCenter);
    dataGroupLayout->setSpacing(4);
    dataGroupLayout->addWidget(new LabeledDataWidget{tr("Peak Press")});
    dataGroupLayout->addWidget(new LabeledDataWidget{tr("Plat Press")});
    dataGroupLayout->addWidget(new LabeledDataWidget{tr("Tidal Vol")});
    dataGroupLayout->addWidget(new LabeledDataWidget{tr("Minute Vent")});
    dataGroupLayout->addWidget(new LabeledDataWidget{tr("Resp Rate")});
    dataGroupLayout->addStretch();

    // Add right column buttons.
    auto buttonGroupWidget = ui->buttonGroupWidget;
    auto buttonGroupLayout = new QVBoxLayout{};
    buttonGroupWidget->setLayout(dataGroupLayout);
    buttonGroupLayout->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    buttonGroupLayout->setSpacing(4);
    dataGroupLayout->addWidget(new PushButtonWidget{tr("Menu")});
    dataGroupLayout->addWidget(new PushButtonWidget{tr("Standby")});

    const qreal TimerInterval_ms = 100;

#if 1
    m_ulParams.xAxisTickCount = 60;
    m_ulParams.xAxisMin = 0.0;
    m_ulParams.xAxisMax = 6.0;
    m_ulParams.yAxisMin = 0.0;
    m_ulParams.yAxisMax = 10.0;
    m_ulParams.yAxisLabel = "Press(cmH2O)";

    m_ulGraph = new GraphWidget(m_ulParams, ui->upperLeftGraphFrame);
#endif

#if 1
    // Upper right graph
    m_urParams.xAxisTickCount = 60;
    m_urParams.xAxisMin = 0.0;
    m_urParams.xAxisMax = 6.0;
    m_urParams.yAxisMin = -1.0;
    m_urParams.yAxisMax = 1.0;
    m_urParams.yAxisLabel = "Flow R(1/min)";

    m_urGraph = new GraphWidget(m_urParams, ui->upperRightGraphFrame);
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
        // Gen cyle of values 0.0 - Y axis max
        static float nextValue = 0.0;
        float v = nextValue;
        nextValue += 1.0;
        nextValue = (nextValue > m_ulParams.yAxisMax) ? 0.0 : nextValue;
        m_ulGraph->onAddValue(v);
    }

    if (nullptr != m_urGraph)
    {
        float v = getSinValue(m_urGraph->getTick(), m_urParams.xAxisTickCount);
        m_urGraph->onAddValue(v);

        // Test out add values
        //static const GraphWidget::FloatVector v = {0.5, 0.0, -0.5, 0.0};
        //m_urGraph->onAddValues(v);
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

float MainWidget::getSinValue(int tick, int tickCount)
{
    // Generate sine wave values between -1.0 and 1.0.
    static constexpr float cycles = 4.0 * 2.0 * M_PI; // 4.0 cycles per screen.
    float input = (cycles * tick) / tickCount;
    float output = qSin(input);

    //qDebug() << "getSinValue() input =" << input << ", output = " << output;
    //qDebug() << "..tick =" << tick << ", tickCount =" << tickCount;
    return output;
}
