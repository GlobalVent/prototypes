#include <QVBoxLayout>
#include <QPushButton>
#include <QtMath>
#include <algorithm>    // std::max

#include <QDebug>

#include "TreatmentWidget.h"
#include "ui_TreatmentWidget.h"
#include "LabeledInputWidget.h"
#include "LabeledDataWidget.h"
#include "GraphWidget.h"
#include "PushButtonWidget.h"
#include "IeRatioSpinBoxWidget.h"

namespace
{
    constexpr const char *FiosLabelStr = QT_TRANSLATE_NOOP("MainWidget", "FiO2");
    constexpr const char *TidalVolLabelStr = QT_TRANSLATE_NOOP("MainWidget", "Tidal Vol.");
    constexpr const char *RespRateLabelStr = QT_TRANSLATE_NOOP("MainWidget", "Resp Rate");
    constexpr const char *IeLabelStr = QT_TRANSLATE_NOOP("MainWidget", "I:E");
    constexpr const char *PeepLabelStr = QT_TRANSLATE_NOOP("MainWidget", "PEEP");
    constexpr const char *VentModeLabelStr = QT_TRANSLATE_NOOP("MainWidget", "Vent Mode");

    constexpr const char *PeakPressLabelStr = QT_TRANSLATE_NOOP("MainWidget", "Peak Press");
    constexpr const char *PlatPressLabelStr = QT_TRANSLATE_NOOP("MainWidget", "Plat Press");
    constexpr const char *MinuteVentLabelStr = QT_TRANSLATE_NOOP("MainWidget", "Minute Vent");

    constexpr const char *PercentSuffixStr = QT_TRANSLATE_NOOP("MainWidget", " %");
    constexpr const char *MlSuffixStr = QT_TRANSLATE_NOOP("MainWidget", " ml");
    constexpr const char *PerMinSuffixStr = QT_TRANSLATE_NOOP("MainWidget", " l/min");
    constexpr const char *Cmh20MinSuffixStr = QT_TRANSLATE_NOOP("MainWidget", " cmH2O");

    constexpr const char *MenuLabelStr = QT_TRANSLATE_NOOP("MainWidget", "Menu");
    constexpr const char *StandbyLabelStr = QT_TRANSLATE_NOOP("MainWidget", "Standby");

    constexpr const char *ValveALabelStr = QT_TRANSLATE_NOOP("MainWidget", "Valve A");
    constexpr const char *ValveBLabelStr = QT_TRANSLATE_NOOP("MainWidget", "Valve B");
    constexpr const char *ValveCLabelStr = QT_TRANSLATE_NOOP("MainWidget", "Valve C");
    constexpr const char *ValveDLabelStr = QT_TRANSLATE_NOOP("MainWidget", "Valve D");
}

TreatmentWidget::TreatmentWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TreatmentWidget)
    , m_ulGraph(nullptr)
    , m_urGraph(nullptr)
    , m_llGraph(nullptr)
    , m_lrGraph(nullptr)
    , m_timerInterval_ms(0)
    , m_timer()
    , m_jamCtrlMgr(nullptr)
{
    ui->setupUi(this);

    // Add bottom row of input widgets
    auto inputGroupFrame = ui->inputGroupFrame;
    auto inputGroupLayout = new QHBoxLayout{};
    inputGroupFrame->setLayout(inputGroupLayout);
    inputGroupLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    inputGroupLayout->setSpacing(24);

    auto w = new LabeledInputWidget(tr(FiosLabelStr));
    m_fiO2SpinBox = w->getSpinBox();
    m_fiO2SpinBox->setRange(21, 100);
    m_fiO2SpinBox->setSuffix(tr(PercentSuffixStr));
    inputGroupLayout->addWidget(w);

    w = new LabeledInputWidget(tr(TidalVolLabelStr));
    m_tidalVolSpinBox = w->getSpinBox();
    //spin->setRange(21, 100);  What is the range?
    m_tidalVolSpinBox->setSuffix(tr(MlSuffixStr));
    inputGroupLayout->addWidget(w);

    w = new LabeledInputWidget(tr(RespRateLabelStr));
    m_respRateSpinBox = w->getSpinBox();
    m_respRateSpinBox->setRange(1, 35);
    m_respRateSpinBox->setSuffix(PerMinSuffixStr);
    inputGroupLayout->addWidget(w);

    // JPW \todo Need to understand how I:E ratio options
    m_ieSpinBox = new IeRatioSpinBoxWidget();
    m_ieSpinBox->setRange(IeRatioSpinBoxWidget::RatioMin, IeRatioSpinBoxWidget::RatioMax);
    w = new LabeledInputWidget(tr(IeLabelStr), m_ieSpinBox);
    inputGroupLayout->addWidget(w);

    w = new LabeledInputWidget(tr(PeepLabelStr));
    m_peepSpinBox = w->getSpinBox();
    m_peepSpinBox->setRange(0, 20);
    m_peepSpinBox->setSuffix(tr(Cmh20MinSuffixStr));
    inputGroupLayout->addWidget(w);

    // JPW \todo Don't know what this is
    inputGroupLayout->addWidget(new LabeledInputWidget(tr(VentModeLabelStr)));

    // Add right column data display widgets
    //auto dataLayout = ui->dataFrame->layout();
    //dataLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    //auto dataFrame = ui->rightColumnFrame;
    auto dataGroupWidget = ui->dataGroupWidget;
    auto dataGroupLayout = new QVBoxLayout{};
    dataGroupWidget->setLayout(dataGroupLayout);
    dataGroupLayout->setAlignment(Qt::AlignTop| Qt::AlignHCenter);
    dataGroupLayout->setSpacing(4);
    dataGroupLayout->addWidget(new LabeledDataWidget(tr(PeakPressLabelStr)));
    dataGroupLayout->addWidget(new LabeledDataWidget(tr(PlatPressLabelStr)));
    dataGroupLayout->addWidget(new LabeledDataWidget(tr(TidalVolLabelStr)));
    dataGroupLayout->addWidget(new LabeledDataWidget(tr(MinuteVentLabelStr)));
    dataGroupLayout->addWidget(new LabeledDataWidget(tr(RespRateLabelStr)));
    dataGroupLayout->addStretch();

    // Add right column buttons.
    auto buttonGroupWidget = ui->buttonGroupWidget;
    auto buttonGroupLayout = new QVBoxLayout{};
    buttonGroupWidget->setLayout(dataGroupLayout);
    buttonGroupLayout->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    buttonGroupLayout->setSpacing(4);
#if 0
    dataGroupLayout->addWidget(new PushButtonWidget(tr(MenuLabelStr)));
    dataGroupLayout->addWidget(new PushButtonWidget(tr(StandbyLabelStr)));
#else
    m_valveAButtonWidget = new PushButtonWidget(tr(ValveALabelStr));
    m_valveAButtonWidget->setCheckable(true);
    dataGroupLayout->addWidget(m_valveAButtonWidget);
    m_valveBButtonWidget = new PushButtonWidget(tr(ValveBLabelStr));
    m_valveBButtonWidget->setCheckable(true);
    dataGroupLayout->addWidget(m_valveBButtonWidget);
    m_valveCButtonWidget = new PushButtonWidget(tr(ValveCLabelStr));
    m_valveCButtonWidget->setCheckable(true);
    dataGroupLayout->addWidget(m_valveCButtonWidget);
    m_valveDButtonWidget = new PushButtonWidget(tr(ValveDLabelStr));
    m_valveDButtonWidget->setCheckable(true);
    dataGroupLayout->addWidget(m_valveDButtonWidget);

    m_standbyButtonWidget = new PushButtonWidget(tr(StandbyLabelStr));
    dataGroupLayout->addWidget(m_standbyButtonWidget);
#endif
    const qreal TimerInterval_ms = 100;

#if 1
    m_ulParams.xAxisTickCount = 120;
    m_ulParams.xAxisMin = 0.0;
    m_ulParams.xAxisMax = 6.0;
    m_ulParams.yAxisMin = 0.0;
    m_ulParams.yAxisMax = 10.0;
    m_ulParams.yAxisLabel = tr("Press(cmH2O)");

    m_ulGraph = new GraphWidget(m_ulParams, ui->upperLeftGraphFrame);
#endif

#if 1
    // Upper right graph
    m_urParams.xAxisTickCount = 120;
    m_urParams.xAxisMin = 0.0;
    m_urParams.xAxisMax = 6.0;
    m_urParams.yAxisMin = -1.0;
    m_urParams.yAxisMax = 1.0;
    m_urParams.yAxisLabel = tr("Flow R(l/min)");

    m_urGraph = new GraphWidget(m_urParams, ui->upperRightGraphFrame);
#endif

#if 1
    // Lower left graph
    m_llParams.xAxisTickCount = 120;
    m_llParams.xAxisMin = 0.0;
    m_llParams.xAxisMax = 6.0;
    m_llParams.yAxisMin = -1.0;
    m_llParams.yAxisMax = 1.0;
    m_llParams.yAxisLabel = tr("pO2");

    m_llGraph = new GraphWidget(m_llParams, ui->lowerLeftGraphFrame);
#endif

#if 1
    // Lower right graph
    m_lrParams.xAxisTickCount = 120;
    m_lrParams.xAxisMin = 0.0;
    m_lrParams.xAxisMax = 6.0;
    m_lrParams.yAxisMin = 0.0;
    m_lrParams.yAxisMax = 10.0;
    m_lrParams.yAxisLabel = tr("Lung Vol");

    m_lrGraph = new GraphWidget(m_lrParams, ui->lowerRightGraphFrame);
#endif

    // Connect signals.
    connect(m_valveAButtonWidget, &QPushButton::toggled, this, &TreatmentWidget::onValveAToggled);
    connect(m_valveBButtonWidget, &QPushButton::toggled, this, &TreatmentWidget::onValveBToggled);
    connect(m_valveCButtonWidget, &QPushButton::toggled, this, &TreatmentWidget::onValveCToggled);
    connect(m_valveDButtonWidget, &QPushButton::toggled, this, &TreatmentWidget::onValveDToggled);

     QObject::connect(&m_timer, &QTimer::timeout, this, &TreatmentWidget::onTimeout);
     m_timer.setInterval(TimerInterval_ms);
     m_timer.start();
}

TreatmentWidget::~TreatmentWidget()
{
    delete ui;
}

UserInputData TreatmentWidget::getUserInputData() {
    UserInputData data;

    data.fiO2 = m_fiO2SpinBox->value();
    data.tidalVol = m_tidalVolSpinBox->value();
    data.respRate = m_respRateSpinBox->value();
    data.ie = static_cast<IeRatioSpinBoxWidget::IeRatio_E>(m_ieSpinBox->value());
    data.peep = m_peepSpinBox->value();

    return data;
}

void TreatmentWidget::setUserInputData(const UserInputData &data)
{
    // \todo Set the widgets from the inputs passed in
}

void TreatmentWidget::keyPressEvent(QKeyEvent *event)
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

void TreatmentWidget::onTimeout()
{
#if 1
    JamCtrlData cd;
    // TBD.. figure out scaling for thes3e
    if (m_jamCtrlMgr)
        cd = m_jamCtrlMgr->getCtrlData();

    // qDebug() << "pRes = " << cd.pRes << "pSys = " << cd.pSys << "pO2 = " << cd .pO2 << "lVol = " << cd.lvol;

    // NOTE: scaling here is really artificial as these are not the data
    //       we are ultimatly graphing, but the data that we are just
    //       using to do a milestone.
    if (m_ulGraph)
        m_ulGraph->onAddValue(std::min(cd.pSys / 3, double(m_ulParams.yAxisMax)));

    if (m_urGraph)
        m_urGraph->onAddValue(std::min((cd.pRes / 1000) + m_urParams.yAxisMin, double(m_urParams.yAxisMax)));

    // @todo Showing raw value. Figure out range and correct.
    if (m_llGraph)
        m_llGraph->onAddValue(cd.pO2);

    // @todo Showing raw value. Figure out range and correct.
    if (m_lrGraph)
        m_lrGraph->onAddValue(cd.lvol);

#else
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
        //m_llGraph->onTimeout();
    }

    if (nullptr != m_lrGraph)
    {
        //m_lrGraph->onTimeout();
    }
#endif
}

void TreatmentWidget::onNameValuePair(QString name, QString value)
{
    //qDebug() << "TreatmentWidget::onNameValuePair(" << name << ", " << value << ")";

    bool isOk;
    int intValue = value.toInt(&isOk, 10);
    if (!isOk)
    {
        qDebug() << "Failed to convert value string to int. value =" << value;
        return;
    }

    if ("L" == name)
    {
        // Low sensor value. Graph on upper left.
        // JPW @todo fill in lower value when know range.
    }
    else if ("H" == name)
    {
        // High sensor value. Graph on upper right.
        float floatValue = static_cast<float>(intValue);
        m_urGraph->onAddValue(floatValue);
    }
}

float TreatmentWidget::getSinValue(int tick, int tickCount)
{
    // Generate sine wave values between -1.0 and 1.0.
    static constexpr float cycles = 4.0 * 2.0 * M_PI; // 4.0 cycles per screen.
    float input = (cycles * tick) / tickCount;
    float output = qSin(input);

    //qDebug() << "getSinValue() input =" << input << ", output = " << output;
    //qDebug() << "..tick =" << tick << ", tickCount =" << tickCount;
    return output;
}

void TreatmentWidget::onValveAToggled(bool isChecked)
{
    if (m_isValueAChecked != isChecked)
    {
        qDebug() << "TreatmentWidget::onValveAToggled(" << isChecked << ")";

        m_isValueAChecked = isChecked;

        m_jamCtrlMgr->setValveAopen(isChecked); // checked = open, unchecked = closed
    }
}

void TreatmentWidget::onValveBToggled(bool isChecked)
{
    if (m_isValueBChecked != isChecked)
    {
        qDebug() << "TreatmentWidget::onValveBToggled(" << isChecked << ")";

        m_isValueBChecked = isChecked;

        m_jamCtrlMgr->setValveBopen(isChecked); // checked = open, unchecked = closed
    }
}

void TreatmentWidget::onValveCToggled(bool isChecked)
{
    if (m_isValueCChecked != isChecked)
    {
        qDebug() << "TreatmentWidget::onValveCToggled(" << isChecked << ")";

        m_isValueCChecked = isChecked;

        m_jamCtrlMgr->setValveCopen(isChecked); // checked = open, unchecked = closed
    }
}

void TreatmentWidget::onValveDToggled(bool isChecked)
{
    if (m_isValueDChecked != isChecked)
    {
        qDebug() << "TreatmentWidget::onValveDToggled(" << isChecked << ")";

        m_isValueDChecked = isChecked;

        m_jamCtrlMgr->setValveDopen(isChecked); // checked = open, unchecked = closed
    }
}
