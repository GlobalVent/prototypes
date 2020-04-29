#include <QtCore/QDebug>
#include <QtCore/QRandomGenerator>
#include <QtMath>

#include "RedGreenWidget.h"

RedGreenWidget::RedGreenWidget(QWidget *parent)
    : QWidget(parent)
    , m_tickCount(24)
    , m_xStep(0)
    , m_tick(0)
    , m_x(0.0)
    , m_lastPoint(0.0, 0.0)
    , m_line()
    , m_timerInterval_ms(200)
{
    // Don't erase previous paints.
    //setAttribute(Qt::WA_NoSystemBackground);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(false);
    setAttribute(Qt::WA_OpaquePaintEvent);

    // Fill the parent.
    resize(parent->size());

    m_xStep = width() / m_tickCount; // Calculate step value for x axis.

    qDebug() << "RedGreenWidget()."
             << "m_tickCount =" << m_tickCount
             << "m_xStep = " << m_xStep
             << "m_timerInterval_ms = " << m_timerInterval_ms;

    // Temporary until controlled with outside timer.
    QObject::connect(&m_timer, &QTimer::timeout, this, &RedGreenWidget::onTimeout);
    m_timer.setInterval(m_timerInterval_ms);
    m_timer.start();
}

void RedGreenWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(*m_pen);
    painter.fillRect(m_x, 0, m_xStep + m_xStep, height(), Qt::black); // Cursor

    // Skip drawing line back from right side to new start on left.
    if (m_line.length() < height())
    {
        painter.drawLine(m_line);
    }
}

void RedGreenWidget::onAddValue(qreal v)
{
   // Switch pens each cycle. And move last point to left side.
  if (0 == m_tick)
  {
      m_pen = (m_pen == &m_redPen)? &m_greenPen : &m_redPen;
  }

  m_x = m_tick * m_xStep;
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
}


void RedGreenWidget::onTimeout()
{
    static int count = 200;

    if (count-- <= 0)
    {
        m_timer.stop();
        return;
    }

    const qreal v = getRandValue();
    // const qreal v = getSinValue();

    //qDebug() << "getSinValue(), v = " << v << ", m_tick = " << m_tick;

    onAddValue(v);
}

qreal RedGreenWidget::getSinValue()
{
   qreal input = ((2.0 * M_PI) * m_tick) / m_tickCount;
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
