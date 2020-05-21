#include <QtCore/QDebug>

#include "Theme.h"
#include "PlotAreaWidget.h"
#include "GraphWidget.h"

namespace {
    constexpr int Indent_px = 20;
}

GraphWidget::GraphWidget(const InitParams &params, QWidget *parent)
    : QWidget(parent)
    , m_params{params}
    , m_plotArea(new PlotAreaWidget(QRect(Indent_px, 0, parent->width() - Indent_px, parent->height() - Indent_px)
    , params.xAxisTickCount, parent))
    , m_yAxisLabel(new VLabelWidget(QRect(0, 0, Indent_px, parent->height() - Indent_px), parent))
    , m_yAxisMaxLabel(new QLabel(parent))
    , m_yAxisMinLabel(new QLabel(parent))
{
    // Fill the parent.
    resize(parent->size());

    m_yAxisMaxLabel->setGeometry(QRect(0, 0, Indent_px, Indent_px));
    QFont font = m_yAxisMaxLabel->font();
    font.setPixelSize(Theme::YAxisLabelFontSize_px);
    m_yAxisMaxLabel->setFont(font);
    m_yAxisMaxLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_yAxisMaxLabel->setText(QString::number(m_params.yAxisMax, 'f', 1));

    m_yAxisMinLabel->setGeometry(QRect(0, height() - Indent_px - Indent_px, Indent_px, Indent_px));
    font = m_yAxisMinLabel->font();
    font.setPixelSize(Theme::YAxisLabelFontSize_px);
    m_yAxisMinLabel->setFont(font);
    m_yAxisMinLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_yAxisMinLabel->setText(QString::number(m_params.yAxisMin, 'f', 1));

    m_yAxisLabel->setText(m_params.yAxisLabel);

    qDebug() << "GraphWidget().";
    qDebug() << "..width() = " << width() << ", height() = " << height();
}

int GraphWidget::getTick()
{
    return m_plotArea->getTick();
}

void GraphWidget::onAddValue(float value)
{
    // Normalize to range of -1.0 to 1.0.
    const float inRange = m_params.yAxisMax - m_params.yAxisMin;

    // Convert to range from 0.0 to 10.0 to -1.0 to 1.0
    float norm = (value - (m_params.yAxisMin)) / inRange * PlotAreaWidget::YRange;
    
    // Convert from 0.0 to 2.0 to -1.0 to 1.0;
    norm = norm + PlotAreaWidget::YMin;

    // qDebug() << "onAddValue(" << value << ") norm = " << norm;

    m_plotArea->onAddValue(norm);
}

#if 0
// Doesn't work correctly as need to repaint after each point added. 
void GraphWidget::onAddValues(const FloatVector& vector)
{
    for (const auto v : vector)
    {
        m_plotArea->onAddValue(v); 
    }
    //m_plotArea->update();
}
#endif