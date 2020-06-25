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
    constexpr const char *FiosLabelStr = QT_TRANSLATE_NOOP("TreatmentWidget", "FiO2");
    constexpr const char *TidalVolLabelStr = QT_TRANSLATE_NOOP("TreatmentWidget", "Tidal Vol.");
    constexpr const char *RespRateLabelStr = QT_TRANSLATE_NOOP("TreatmentWidget", "Resp Rate");
    constexpr const char *IeLabelStr = QT_TRANSLATE_NOOP("TreatmentWidget", "I:E");
    constexpr const char *PeepLabelStr = QT_TRANSLATE_NOOP("TreatmentWidget", "PEEP");
    constexpr const char *VentModeLabelStr = QT_TRANSLATE_NOOP("TreatmentWidget", "Vent Mode");

    constexpr const char *PeakPressLabelStr = QT_TRANSLATE_NOOP("TreatmentWidget", "Peak Press");
    constexpr const char *PlatPressLabelStr = QT_TRANSLATE_NOOP("TreatmentWidget", "Plat Press");
    constexpr const char *MinuteVentLabelStr = QT_TRANSLATE_NOOP("TreatmentWidget", "Minute Vent");

    constexpr const char *PercentSuffixStr = QT_TRANSLATE_NOOP("TreatmentWidget", " %");
    constexpr const char *MlSuffixStr = QT_TRANSLATE_NOOP("TreatmentWidget", " ml");
    constexpr const char *PerMinSuffixStr = QT_TRANSLATE_NOOP("TreatmentWidget", " l/min");
    constexpr const char *Cmh20MinSuffixStr = QT_TRANSLATE_NOOP("TreatmentWidget", " cmH2O");

    constexpr const char *MenuLabelStr = QT_TRANSLATE_NOOP("TreatmentWidget", "Menu");
    //constexpr const char *StandbyLabelStr = QT_TRANSLATE_NOOP("TreatmentWidget", "Standby");
    constexpr const char *PowerupLabelStr = QT_TRANSLATE_NOOP("MainWidget", "Powerup");  // For now use to go to Powerup screen.
}

TreatmentWidget::TreatmentWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TreatmentWidget)
    , m_ulGraph(nullptr)
    , m_urGraph(nullptr)
    , m_llGraph(nullptr)
    , m_lrGraph(nullptr)
{
    ui->setupUi(this);

    // Add bottom row of input widgets
    auto inputGroupFrame = ui->inputGroupFrame;
    auto inputGroupLayout = new QHBoxLayout{};
    inputGroupFrame->setLayout(inputGroupLayout);
    inputGroupLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    inputGroupLayout->setSpacing(24);

    auto w = new LabeledInputWidget(tr(FiosLabelStr));
    m_fio2SpinBox = w->getSpinBox();
    m_fio2SpinBox->setRange(21, 100);
    m_fio2SpinBox->setValue(100);
    m_fio2SpinBox->setSuffix(tr(PercentSuffixStr));
    inputGroupLayout->addWidget(w);

    w = new LabeledInputWidget(tr(TidalVolLabelStr));
    m_tidalVolSpinBox = w->getSpinBox();
    // JPW @todo What is the range for this input?
    m_tidalVolSpinBox->setSuffix(tr(MlSuffixStr));
    inputGroupLayout->addWidget(w);

    w = new LabeledInputWidget(tr(RespRateLabelStr));
    m_respRateSpinBox = w->getSpinBox();
    m_respRateSpinBox->setRange(1, 35);
    m_respRateSpinBox->setSuffix(PerMinSuffixStr);
    inputGroupLayout->addWidget(w);

    // JPW \todo Need to understand how I:E ratio options
    m_ieRatioSpinBox = new IeRatioSpinBoxWidget();
    m_ieRatioSpinBox->setRange(IeRatioSpinBoxWidget::RatioMin, IeRatioSpinBoxWidget::RatioMax);
    w = new LabeledInputWidget(tr(IeLabelStr), m_ieRatioSpinBox);
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
    m_powerupButtonWidget = new PushButtonWidget(tr(PowerupLabelStr));
    dataGroupLayout->addWidget(m_powerupButtonWidget);
#endif

#if 1
    m_ulParams.xAxisTickCount = 120;
    m_ulParams.xAxisMin = 0.0;
    m_ulParams.xAxisMax = 6.0;
    m_ulParams.yAxisMin = 0.0;
    m_ulParams.yAxisMax = 10.0;
    m_ulParams.yAxisLabel = tr("Press(cmH2O) L");

    m_ulGraph = new GraphWidget(m_ulParams, ui->upperLeftGraphFrame);
#endif

#if 1
    // Upper right graph
    m_urParams.xAxisTickCount = 120;
    m_urParams.xAxisMin = 0.0;
    m_urParams.xAxisMax = 6.0;
    m_urParams.yAxisMin = -1.0;
    m_urParams.yAxisMax = 1.0;
    m_urParams.yAxisLabel = tr("Flow R(l/min) H");

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
    // Inputs
    connect(m_fio2SpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &TreatmentWidget::onFio2Changed);
    connect(m_tidalVolSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &TreatmentWidget::onTidalVolChanged);
    connect(m_respRateSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &TreatmentWidget::onRespRateChanged);
    connect(m_ieRatioSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &TreatmentWidget::onIeRatioChanged);
    connect(m_peepSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &TreatmentWidget::onPeepChanged);

    // Buttons
    connect(m_powerupButtonWidget, &QPushButton::clicked, this, &TreatmentWidget::sigPowerupButtonClicked);
}

TreatmentWidget::~TreatmentWidget()
{
    delete ui;
}

UserInputData TreatmentWidget::getUserInputData() {
    UserInputData data;

    data.fiO2 = m_fio2SpinBox->value();
    data.tidalVol = m_tidalVolSpinBox->value();
    data.respRate = m_respRateSpinBox->value();
    data.ie = static_cast<IeRatioSpinBoxWidget::IeRatio_E>(m_ieRatioSpinBox->value());
    data.peep = m_peepSpinBox->value();

    return data;
}

void TreatmentWidget::setUserInputData(const UserInputData &data)
{
    // \todo Set the widgets from the inputs passed in
}

void TreatmentWidget::onNewInData(CommMgr::DataIn data)
{
    // Upper left graph
    if (nullptr != m_ulGraph)
    {
        //CommMgr::NumType v = std::min(data.pressSys / 3, static_cast<CommMgr ::NumType>(m_ulParams.yAxisMax));
        CommMgr::NumType v = data.pressSys / 3;  // Should clamp. No need for min();
        m_ulGraph->onAddValue(v);
    }

    // Upper right graph
    if (nullptr != m_urGraph)
    {
        //CommMgr::NumType v = std::min((data.pressRes / 1000) + m_urParams.yAxisMin, static_cast<CommMgr ::NumType>(m_urParams.yAxisMax));
        CommMgr::NumType v = (data.pressRes / 1000) + m_urParams.yAxisMin; // Should clamp. No need for min();
        m_urGraph->onAddValue(v);

        // qDebug() << "data.pressRes = " << data.pressRes << ", v = " << v;
    }

    // Lower left graph
    if (nullptr != m_llGraph)
    {
        CommMgr::NumType v = data.pressO2;
        m_llGraph->onAddValue(v);
    }

    // Lower right graph
    if (nullptr != m_lrGraph)
    {
        CommMgr::NumType v = data.lungVol;
        m_lrGraph->onAddValue(v);
    }
}

void TreatmentWidget::onFio2Changed(int value)
{
    qDebug() << "TreatmentWidget::onFio2Changed(" << value << ") called.";
    emit sigFio2Changed(value);
}

void TreatmentWidget::onTidalVolChanged(int value)
{
    qDebug() << "TreatmentWidget::onTidalVolChanged(" << value << ") called.";
    emit sigTidalVolChanged(value);
}

void TreatmentWidget::onRespRateChanged(int value)
{
    qDebug() << "TreatmentWidget::onRespRateChanged(" << value << ") called.";
    emit sigRespRateChanged(value);
}

void TreatmentWidget::onIeRatioChanged(int value)
{
    qDebug() << "TreatmentWidget::onIeRatioChanged(" << value << ") called.";
    emit sigIeRatioChanged(value);
}

void TreatmentWidget::onPeepChanged(int value)
{
    qDebug() << "TreatmentWidget::onPeepChanged(" << value << ") called.";
    emit sigPeepChanged(value);
}
