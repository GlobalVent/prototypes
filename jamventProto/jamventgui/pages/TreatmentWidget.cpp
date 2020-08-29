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
#include "ConfigJson.h"

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

    // Configuration data used to setup widgets
    const ConfigJson::ConfigData& configData = ConfigJson::Instance().getConfigData();

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
    m_tidalVolSpinBox->setRange(10, 1500);
    m_tidalVolSpinBox->setSingleStep(10);
    m_tidalVolSpinBox->setValue(350);
    m_tidalVolSpinBox->setSuffix(tr(MlSuffixStr));
    inputGroupLayout->addWidget(w);

    w = new LabeledInputWidget(tr(RespRateLabelStr));
    m_respRateSpinBox = w->getSpinBox();
    m_respRateSpinBox->setRange(1, 40);
    m_respRateSpinBox->setValue(10);
    m_respRateSpinBox->setSuffix(PerMinSuffixStr);
    inputGroupLayout->addWidget(w);

    m_ieRatioSpinBox = new IeRatioSpinBoxWidget();
    m_ieRatioSpinBox->setRange(IeRatioSpinBoxWidget::RatioMin, IeRatioSpinBoxWidget::RatioMax);
    m_ieRatioSpinBox->setValue(IeRatioSpinBoxWidget::Ratio1to1);
    w = new LabeledInputWidget(tr(IeLabelStr), m_ieRatioSpinBox);
    inputGroupLayout->addWidget(w);

    w = new LabeledInputWidget(tr(PeepLabelStr));
    m_peepSpinBox = w->getSpinBox();
    m_peepSpinBox->setRange(0, 20);
    m_peepSpinBox->setValue(5);
    m_peepSpinBox->setSuffix(tr(Cmh20MinSuffixStr));
    inputGroupLayout->addWidget(w);

    // JPW @todo  Just a placeholder for possible modes PRVS, Support 
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

    // Configure the 4 graphs
#if 1
    m_ulParams.xAxisTickCount = configData.graphTickCount;
    m_ulParams.xAxisMin = configData.graphs[ConfigJson::UlIndex].xAxisMin;
    m_ulParams.xAxisMax = configData.graphs[ConfigJson::UlIndex].xAxisMax;
    m_ulParams.yAxisMin = configData.graphs[ConfigJson::UlIndex].yAxisMin;
    m_ulParams.yAxisMax = configData.graphs[ConfigJson::UlIndex].yAxisMax;
    m_ulParams.yAxisLabel = configData.graphs[ConfigJson::UlIndex].yAxisLabel;

    m_ulGraph = new GraphWidget(m_ulParams, ui->upperLeftGraphFrame);
#endif

#if 1
    // Upper right graph
    m_urParams.xAxisTickCount = configData.graphTickCount;
    m_urParams.xAxisMin = configData.graphs[ConfigJson::UrIndex].xAxisMin;
    m_urParams.xAxisMax = configData.graphs[ConfigJson::UrIndex].xAxisMax;
    m_urParams.yAxisMin = configData.graphs[ConfigJson::UrIndex].yAxisMin;
    m_urParams.yAxisMax = configData.graphs[ConfigJson::UrIndex].yAxisMax;
    m_urParams.yAxisLabel = configData.graphs[ConfigJson::UrIndex].yAxisLabel;

    m_urGraph = new GraphWidget(m_urParams, ui->upperRightGraphFrame);
#endif

#if 1
    // Lower left graph
    m_llParams.xAxisTickCount = configData.graphTickCount;
    m_llParams.xAxisMin = configData.graphs[ConfigJson::LlIndex].xAxisMin;
    m_llParams.xAxisMax = configData.graphs[ConfigJson::LlIndex].xAxisMax;
    m_llParams.yAxisMin = configData.graphs[ConfigJson::LlIndex].yAxisMin;
    m_llParams.yAxisMax = configData.graphs[ConfigJson::LlIndex].yAxisMax;
    m_llParams.yAxisLabel = configData.graphs[ConfigJson::LlIndex].yAxisLabel;

    m_llGraph = new GraphWidget(m_llParams, ui->lowerLeftGraphFrame);
#endif

#if 1
    // Lower right graph
    m_lrParams.xAxisTickCount = configData.graphTickCount;
    m_lrParams.xAxisMin = configData.graphs[ConfigJson::LrIndex].xAxisMin;
    m_lrParams.xAxisMax = configData.graphs[ConfigJson::LrIndex].xAxisMax;
    m_lrParams.yAxisMin = configData.graphs[ConfigJson::LrIndex].yAxisMin;
    m_lrParams.yAxisMax = configData.graphs[ConfigJson::LrIndex].yAxisMax;
    m_lrParams.yAxisLabel = configData.graphs[ConfigJson::LrIndex].yAxisLabel;

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
        // Convert from Bar to cmH2O to display.
        JamCtrlMgr::FloatNumType v_cmh2o = JamCtrlMgr::BarToCmh2o(data.pSys_bar);
        //qDebug() << "data.pSys_bar = " << data.pSys_bar << ", v_cmh2o = " << v_cmh2o;
        m_ulGraph->onAddValue(v_cmh2o);
    }

    // Upper right graph
    if (nullptr != m_urGraph)
    {
        // Convert from Bar to cmH2O to display.
        JamCtrlMgr::FloatNumType v_cmh2o = JamCtrlMgr::BarToCmh2o(data.pRes_bar);
        //qDebug() << "data.pRes_bar = " << data.pRes_bar << ", v_cmh2o = " << v_cmh2o;
        // JPW @todo showing in bars as that matches graph range for now.  Change from 0.0 to 2.0 -> -1.0 to 1.0
        m_urGraph->onAddValue(data.pRes_bar - 1.0);
    }

    // Lower left graph
    if (nullptr != m_llGraph)
    {
        m_llGraph->onAddValue(data.o2_pc);
    }

    // Lower right graph
    if (nullptr != m_lrGraph)
    {
        m_lrGraph->onAddValue(data.lungVol_ml);
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
