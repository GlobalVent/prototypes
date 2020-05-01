#include <QtCore/QDebug>
#include <QtCore/QRandomGenerator>
#include <QtMath>

#include "RedGreenWidget.h"

RedGreenWidget::RedGreenWidget(const InitParams &params, QWidget *parent)
    : QWidget(parent)
    , m_tickCount(params.xAxisTickCount)
    , m_xStep(0.0)
    , m_tick(0)
    , m_x(0.0)
    , m_lastPoint(0.0, 0.0)
    , m_line()
{
    // Don't erase previous paints.
    //setAttribute(Qt::WA_NoSystemBackground);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(false);
    setAttribute(Qt::WA_OpaquePaintEvent);

    // Fill the parent.
    resize(parent->size());

    m_xStep = static_cast<float>(width()) / m_tickCount; // Calculate as real as need percision.

    qDebug() << "RedGreenWidget()."
             << "m_tickCount =" << m_tickCount
             << "m_xStep = " << m_xStep;

    qDebug() << "..width() = " << width() << ", height() = " << height();
}

void RedGreenWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(*m_pen);

    // Skip drawing line back from right side to new start on left.
    if (m_line.length() < height())
    {
        // Cursor
        const int stepWidth = static_cast<int>(m_xStep);
        painter.fillRect(m_x + 1, 0, stepWidth + stepWidth, height(), Qt::black);
        //painter.fillRect(m_x + 1, 0, 1, height(), Qt::white);

        painter.drawLine(m_line);
    }

    //qDebug() << "paintEvent(). m_line = " << m_line << ", m_x = " << m_x;
}

void RedGreenWidget::onAddValue(qreal v)
{
   // Switch pens each cycle. And move last point to left side.
  if (0 == m_tick)
  {
      m_pen = (m_pen == &m_redPen)? &m_greenPen : &m_redPen;
  }

  m_x = static_cast<int>(m_xStep * m_tick);
  ++m_tick;
  if (m_tick > (m_tickCount + 1))
  {
      // Did the last rightmost point.  Start back on the left.
      m_tick = 0;
  }

  const auto h = height(); 
  QPointF newPoint(m_x, (v * h * 0.4) + (h * 0.5));
  m_line = QLineF(m_lastPoint, newPoint);
  m_lastPoint = newPoint;
  update();

  //qDebug() << "onAddValue(). newPoint = " << newPoint << ", m_x = " << m_x;
}


void RedGreenWidget::onTimeout()
{
    //const qreal v = getRandValue();
    const qreal v = getSinValue();

    //qDebug() << "getSinValue(), v = " << v << ", m_tick = " << m_tick;

    onAddValue(v);
}

qreal RedGreenWidget::getSinValue()
{
    static constexpr qreal cycles = 4.0 * 2.0 * M_PI;  // 4.0 cycles per screen.
    qreal input = (cycles * m_tick) / m_tickCount;
    qreal output = qSin(input);

    // qDebug() << "getSinValue() input =" << input << ", output = " << output;

    return output;
}

qreal RedGreenWidget::getRandValue()
{
    // Random value between -1.0 and 1.0.
    qreal output = QRandomGenerator::global()->bounded(1.0 + 1.0) - 1.0;

    // qDebug() << "getRandValue() output = " << output;

    return output;
}
