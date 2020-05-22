#include <QVBoxLayout>

#include "Theme.h"
#include "LabeledDataWidget.h"

namespace
{
    const QString LabelStyle = "QLabel { font-family: 'Roboto'; font-size: 14px; font-weight: 700; color: black; }";
    const QString DataLabelStyle = "QLabel { font-family: 'Roboto'; font-size: 14px; font-weight: 700; color: black;  border: 2px solid blue;}";
}

LabeledDataWidget::LabeledDataWidget(const QString& label, QWidget* parent)
    : QWidget(parent)
    , m_label{new QLabel{label}}
    , m_dataLabel{new QLabel}
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

    m_dataLabel->setMinimumWidth(80);
    layout->addWidget(m_dataLabel);

    m_label->setStyleSheet(LabelStyle);
    m_dataLabel->setStyleSheet(DataLabelStyle);

    m_label->setFont(Theme::MainFont);
    m_dataLabel->setFont(Theme::MainFont);
}

QLabel* LabeledDataWidget::getDataLabel()
{
    return m_dataLabel;
}
