#include <QtCore/QDebug>

#include "Theme.h"
#include "PlotAreaWidget.h"
#include "GraphWidget.h"

namespace {
    constexpr int Indent_px = 24;
}

GraphWidget::GraphWidget(const InitParams &params, QWidget *parent)
    : QWidget(parent)
    , m_params{params}
    , m_plotArea(new PlotAreaWidget(QRect(Indent_px, 0, parent->width() - Indent_px, parent->height())
    , params.xAxisTickCount, parent))
    , m_yAxisLabel(new VLabelWidget(QRect(0, 0, Indent_px, parent->height()), parent))
    , m_yAxisMaxLabel(new QLabel(parent))
    , m_yAxisMinLabel(new QLabel(parent))
{
    // Fill the parent.
    resize(parent->size());

    m_yAxisMaxLabel->setGeometry(QRect(0, 0, Indent_px, Indent_px));
    QFont font = m_yAxisMaxLabel->font();
    font.setPixelSize(Theme::SmallerFontSize_px);
    m_yAxisMaxLabel->setFont(font);
    m_yAxisMaxLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_yAxisMaxLabel->setText(QString::number(m_params.yAxisMax, 'f', 1));

    m_yAxisMinLabel->setGeometry(QRect(0, height() - Indent_px, Indent_px, Indent_px));
    font = m_yAxisMinLabel->font();
    font.setPixelSize(Theme::SmallerFontSize_px);
    m_yAxisMinLabel->setFont(font);
    m_yAxisMinLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_yAxisMinLabel->setText(QString::number(m_params.yAxisMin, 'f', 1));

    m_yAxisLabel->setText(m_params.yAxisLabel);

    // If graph y range contains 0, set the place to draw the zero line.
    if ((0.0 >= m_params.yAxisMin) && (0.0 <= m_params.yAxisMax))
    {
        // Zero (0.0) is in the y range. Set the value to draw the zero line.
        const float inRange = m_params.yAxisMax - m_params.yAxisMin;

        // Convert from input range to range -1.0 to 1.0.
        float yZero = (0.0 - m_params.yAxisMin) / inRange * PlotAreaWidget::YRange;
        yZero = yZero + PlotAreaWidget::YMin;

        m_plotArea->setZeroLine(yZero);
    }

    qDebug() << "GraphWidget().";
    qDebug() << "..width() = " << width() << ", height() = " << height();
}

int GraphWidget::getTick()
{
    return m_plotArea->getTick();
}

void GraphWidget::onAddValue(float value)
{
    const float inRange = m_params.yAxisMax - m_params.yAxisMin;

    // Normalize to range of -1.0 to 1.0.
    float norm;

    // Force value to be in range.
    if (value < m_params.yAxisMin)
    {
        norm = m_params.yAxisMin;
    }
    else if (value > m_params.yAxisMax)
    {
        norm = m_params.yAxisMax;
    }
    else
    {
        norm = value;
    }

     // Convert to range the input range to -1.0 to 1.0
     // Converts to range of 0.0 to 2.0;
     norm = (norm - m_params.yAxisMin) / inRange * PlotAreaWidget::YRange;

     // Convert from 0.0 to 2.0 to -1.0 to 1.0;
     norm = norm + PlotAreaWidget::YMin;

     // qDebug() << "onAddValue(" << value << ") norm = " << norm;

     m_plotArea->onAddValue(norm);
}
