#include <QVBoxLayout>
#include <QPushButton>

#include <QDebug>

#include "MainCtrl.h"
#include "LabeledInputWidget.h"
#include "PushButtonWidget.h"
#include "ui_PowerupWidget.h"

namespace
{
    constexpr const char *MaxPressLabelStr = QT_TRANSLATE_NOOP("PowerupWidget", "Max Press");
    constexpr const char *VaTargetLabelStr = QT_TRANSLATE_NOOP("PowerupWidget", "Va Target");
    constexpr const char *VbTargetLabelStr = QT_TRANSLATE_NOOP("PowerupWidget", "Vb target");
    constexpr const char *SysTargetLabelStr = QT_TRANSLATE_NOOP("PowerupWidget", "Sys Target");

    constexpr const char *ValveALabelStr = QT_TRANSLATE_NOOP("PowerupWidget", "Open Valve A");
    constexpr const char *ValveBLabelStr = QT_TRANSLATE_NOOP("PowerupWidget", "Open Valve B");
    constexpr const char *ValveCLabelStr = QT_TRANSLATE_NOOP("PowerupWidget", "Open Valve C");
    constexpr const char *ValveDLabelStr = QT_TRANSLATE_NOOP("PowerupWidget", "Open Valve D");

    constexpr const char *UseSerialButtonStr = QT_TRANSLATE_NOOP("PowerupWidget", "Use Serial");
    constexpr const char *SimOnButtonStr = QT_TRANSLATE_NOOP("PowerupWidget", "Sim On");
    constexpr const char *TreatmentButtonStr = QT_TRANSLATE_NOOP("PowerupWidget", "Treatment");
}

PowerupWidget::PowerupWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PowerupWidget)
{
    ui->setupUi(this);

    ui->textEditWidget->setText("Power Up Page: (Esc to exit application)");

    // Add bottom row of input widgets
    auto inputGroupFrame = ui->inputGroupFrame;
    auto inputGroupLayout = new QHBoxLayout{};
    inputGroupFrame->setLayout(inputGroupLayout);
    inputGroupLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    inputGroupLayout->setSpacing(24);

    auto w = new LabeledInputWidget(tr(MaxPressLabelStr));
    m_maxPressSpinBox = w->getSpinBox();
    //m_maxPressSpinBox->setRange(21, 100);
    //m_maxPressSpinBox->setValue(100);
    inputGroupLayout->addWidget(w);

    w = new LabeledInputWidget(tr(VaTargetLabelStr));
    m_vaTargetSpinBox = w->getSpinBox();
    inputGroupLayout->addWidget(w);
    w = new LabeledInputWidget(tr(VbTargetLabelStr));
    m_vbTargetSpinBox = w->getSpinBox();
    inputGroupLayout->addWidget(w);
    w = new LabeledInputWidget(tr(SysTargetLabelStr));
    m_sysTargetSpinBox = w->getSpinBox();
    inputGroupLayout->addWidget(w);

    // Add right column buttons.
    auto dataGroupWidget = ui->dataGroupWidget;
    auto dataGroupLayout = new QVBoxLayout{};
    auto buttonGroupWidget = ui->buttonGroupWidget;
    auto buttonGroupLayout = new QVBoxLayout{};
    buttonGroupWidget->setLayout(buttonGroupLayout);
    buttonGroupLayout->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    buttonGroupLayout->setSpacing(4);

    m_valveAButtonWidget = new PushButtonWidget(tr(ValveALabelStr));
    m_valveAButtonWidget->setCheckable(true);
    buttonGroupLayout->addWidget(m_valveAButtonWidget);
    m_valveBButtonWidget = new PushButtonWidget(tr(ValveBLabelStr));
    m_valveBButtonWidget->setCheckable(true);
    buttonGroupLayout->addWidget(m_valveBButtonWidget);
    m_valveCButtonWidget = new PushButtonWidget(tr(ValveCLabelStr));
    m_valveCButtonWidget->setCheckable(true);
    buttonGroupLayout->addWidget(m_valveCButtonWidget);
    m_valveDButtonWidget = new PushButtonWidget(tr(ValveDLabelStr));
    m_valveDButtonWidget->setCheckable(true);
    buttonGroupLayout->addWidget(m_valveDButtonWidget);

    m_useSerialButtonWidget = new PushButtonWidget(tr(UseSerialButtonStr));
    m_useSerialButtonWidget->setCheckable(true);
    buttonGroupLayout->addWidget(m_useSerialButtonWidget);

    m_simOnButtonWidget = new PushButtonWidget(tr(SimOnButtonStr));
    m_simOnButtonWidget->setCheckable(true);
    buttonGroupLayout->addWidget(m_simOnButtonWidget);

    m_treatmentButtonWidget = new PushButtonWidget(tr(TreatmentButtonStr));
    buttonGroupLayout->addWidget(m_treatmentButtonWidget);

    // Connect signals.

    // Valve buttons on right column
    connect(m_valveAButtonWidget, &QPushButton::toggled, this, &PowerupWidget::onValveAChanged);
    connect(m_valveBButtonWidget, &QPushButton::toggled, this, &PowerupWidget::onValveBChanged);
    connect(m_valveCButtonWidget, &QPushButton::toggled, this, &PowerupWidget::onValveCChanged);
    connect(m_valveDButtonWidget, &QPushButton::toggled, this, &PowerupWidget::onValveDChanged);

    // Buttons on right column
    connect(m_useSerialButtonWidget, &QPushButton::toggled, this, &PowerupWidget::onUseSerialChanged);
    connect(m_simOnButtonWidget, &QPushButton::toggled, this, &PowerupWidget::onSimOnChanged);
    connect(m_treatmentButtonWidget, &QPushButton::clicked, this, &PowerupWidget::sigTreatmentButtonClicked);

    // Inputs for demo phase
    connect(m_maxPressSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &PowerupWidget::onMaxPressChanged);
    connect(m_vaTargetSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &PowerupWidget::onVaTargetChanged);
    connect(m_vbTargetSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &PowerupWidget::onVbTargetChanged);
    connect(m_sysTargetSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &PowerupWidget::onSysTargetChanged);
}

PowerupWidget::~PowerupWidget()
{
    delete ui;
}

void PowerupWidget::onValveAChanged(bool isChecked)
{
    if (m_isValueAChecked != isChecked)
    {
        qDebug() << "TreatmentWidget::onValveAToggled(" << isChecked << ")";
        m_isValueAChecked = isChecked;
        emit sigValueAOpenChanged(isChecked);
    }
}

void PowerupWidget::onValveBChanged(bool isChecked)
{
    if (m_isValueBChecked != isChecked)
    {
        qDebug() << "TreatmentWidget::onValveBToggled(" << isChecked << ")";
        m_isValueBChecked = isChecked;
        emit sigValueBOpenChanged(isChecked);
    }
}

void PowerupWidget::onValveCChanged(bool isChecked)
{
    if (m_isValueCChecked != isChecked)
    {
        qDebug() << "TreatmentWidget::onValveCToggled(" << isChecked << ")";
        m_isValueCChecked = isChecked;
        emit sigValueCOpenChanged(isChecked);
    }
}

void PowerupWidget::onValveDChanged(bool isChecked)
{
    if (m_isValueDChecked != isChecked)
    {
        qDebug() << "TreatmentWidget::onValveDToggled(" << isChecked << ")";
        m_isValueDChecked = isChecked;
        emit sigValueDOpenChanged(isChecked);
    }
}

void PowerupWidget::onMaxPressChanged(int value)
{
    qDebug() << "PowerupWidget::onMaxPressChanged(" << value << ") called.";
    emit sigMaxPressChanged(value);
}

void PowerupWidget::onVaTargetChanged(int value)
{
    qDebug() << "PowerupWidget::onVaTargetChanged(" << value << ") called.";
    emit sigVaTargetChanged(value);
}

void PowerupWidget::onVbTargetChanged(int value)
{
    qDebug() << "PowerupWidget::onVbTargetChanged(" << value << ") called.";
    emit sigVbTargetChanged(value);
}

void PowerupWidget::onSysTargetChanged(int value)
{
    qDebug() << "PowerupWidget::onSysTargetChanged(" << value << ") called.";
    emit sigSysTargetChanged(value);
}

void PowerupWidget::onUseSerialChanged(bool isChecked)
{
    if (m_isUseSerialChecked != isChecked)
    {
        qDebug() << "PowerupWidget::onUseSerialChanged(" << isChecked << ")";
        m_isUseSerialChecked = isChecked;
        emit sigUseSerialChanged(isChecked);
    }
}

void PowerupWidget::onSimOnChanged(bool isChecked)
{
    if (m_isSimOnChecked != isChecked)
    {
        qDebug() << "PowerupWidget::onSimOnChanged(" << isChecked << ")";
        m_isSimOnChecked = isChecked;
        emit sigSimOnChanged(isChecked);
    }
}
