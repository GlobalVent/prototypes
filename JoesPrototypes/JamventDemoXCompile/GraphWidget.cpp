#include <QtCore/QDebug>
#include <QtCore/QRandomGenerator>
#include <QtMath>

#include "PlotAreaWidget.h"
#include "GraphWidget.h"

namespace {
    constexpr int Indent_px = 20;
}

GraphWidget::GraphWidget(const InitParams &params, QWidget *parent)
    : QWidget(parent), m_params{params}
    , m_plotArea(new PlotAreaWidget(QRect(Indent_px, 0, parent->width() - Indent_px, parent->height() - Indent_px), params.xAxisTickCount, parent))
    , m_yAxisLabel(new VLabelWidget(QRect(0, 0, Indent_px, parent->height()), parent))
    , m_yAxisMaxLabel(new QLabel(parent))
{
    // Fill the parent.
    resize(parent->size());

    m_yAxisMaxLabel->setGeometry(QRect(0, 0, Indent_px, Indent_px));
    QFont font = m_yAxisMaxLabel->font();
    font.setPixelSize(11);
    m_yAxisMaxLabel->setFont(font);
    m_yAxisMaxLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    // Dummy text for now
    m_yAxisLabel->setText("Press(cmH20)");
    m_yAxisMaxLabel->setText("6.0");

    qDebug() << "GraphWidget().";
    qDebug() << "..width() = " << width() << ", height() = " << height();
}

void GraphWidget::onAddValue(qreal v)
{
    m_plotArea->onAddValue(v);
}


void GraphWidget::onTimeout()
{
    //const qreal v = getRandValue();
    const qreal v = getSinValue(m_plotArea->getTick(), m_plotArea->getTickCount());

    //qDebug() << "getSinValue(), v = " << v;

    m_plotArea->onAddValue(v);
}

qreal GraphWidget::getSinValue(int tick, int tickCount)
{
    // Generate sine wave values between -1.0 and 1.0.
    static constexpr qreal cycles = 4.0 * 2.0 * M_PI;  // 4.0 cycles per screen.
    qreal input = (cycles * tick) / tickCount;
    qreal output = qSin(input);

    //qDebug() << "getSinValue() input =" << input << ", output = " << output;
    //qDebug() << "..tick =" << tick << ", tickCount =" << tickCount;
    return output;
}

qreal GraphWidget::getRandValue()
{
    // Random value between -1.0 and 1.0.
    qreal output = QRandomGenerator::global()->bounded(1.0 + 1.0) - 1.0;

    // qDebug() << "getRandValue() output = " << output;

    return output;
}
