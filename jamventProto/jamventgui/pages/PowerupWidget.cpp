#include <QVBoxLayout>
#include <QPushButton>

#include <QDebug>

#include "MainCtrl.h"
#include "ui_PowerupWidget.h"

namespace
{
    constexpr const char *TreatmentButtonStr = QT_TRANSLATE_NOOP("PowerupWidget", "Treatment");
}

PowerupWidget::PowerupWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PowerupWidget)
{
    ui->setupUi(this);

    ui->textEditWidget->setText("Power Up Page:");

    auto dataGroupWidget = ui->dataGroupWidget;
    auto dataGroupLayout = new QVBoxLayout{};

    // Add right column buttons.
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
