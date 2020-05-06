
#include "LabeledDataWidget.h"

LabeledDataWidget::LabeledDataWidget(const QString& label, QWidget* parent)
    : QWidget(parent)
    , m_label{new QLabel{label}}
    , m_dataLabel{new QLabel}
{
}

QLabel* LabeledDataWidget::getDataLabel()
{
    return m_dataLabel;
}
