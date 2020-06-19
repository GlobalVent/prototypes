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

    constexpr const char *TreatmentButtonStr = QT_TRANSLATE_NOOP("PowerupWidget", "Treatment");
}

PowerupWidget::PowerupWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PowerupWidget)
{
    ui->setupUi(this);

    ui->textEditWidget->setText("Power Up Page:");

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

    // Add right column buttons.
    auto dataGroupWidget = ui->dataGroupWidget;
    auto dataGroupLayout = new QVBoxLayout{};
    auto buttonGroupWidget = ui->buttonGroupWidget;
    auto buttonGroupLayout = new QVBoxLayout{};
    buttonGroupWidget->setLayout(buttonGroupLayout);
    buttonGroupLayout->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    buttonGroupLayout->setSpacing(4);
    m_treatmentButtonWidget = new PushButtonWidget(tr(TreatmentButtonStr));
    buttonGroupLayout->addWidget(m_treatmentButtonWidget);

    // Connect signals.
    connect(m_treatmentButtonWidget, &QPushButton::clicked, this, &PowerupWidget::sigTreatmentButtonClicked);
}

PowerupWidget::~PowerupWidget()
{
    delete ui;
}
