#include <QDebug>
#include <QVBoxLayout>

#include "Theme.h"
#include "LabeledInputWidget.h"

namespace
{
    const QString LabelStyle = "QLabel { font-family: 'Roboto'; font-size: 14px; font-weight: 500; color: black; }";
}

LabeledInputWidget::LabeledInputWidget(const QString& label, QWidget* parent)
    : QWidget(parent)
    , m_label{new QLabel{label}}
    , m_spinBox{new SpinBoxWidget}
{
    init();
}

LabeledInputWidget::LabeledInputWidget(const QString &label, SpinBoxWidget *spinBox, QWidget *parent)
    : QWidget(parent), m_label{new QLabel{label}}, m_spinBox{spinBox}
{
    init();
}

void LabeledInputWidget::init()
{
    // Set size policy
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    setFixedHeight(48);
    //setMaximumWidth(120);
    setMinimumWidth(60);

    // Add to a horizontal layout.
    auto layout = new QVBoxLayout{};
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    layout->setSpacing(2);
    setLayout(layout);

    layout->addWidget(m_label);

    m_spinBox->setMinimumWidth(80);
    layout->addWidget(m_spinBox);

    setStyleSheet(LabelStyle);

    m_label->setFont(Theme::MainFont);
}

QSpinBox* LabeledInputWidget::getSpinBox()
{
    return m_spinBox;
}
