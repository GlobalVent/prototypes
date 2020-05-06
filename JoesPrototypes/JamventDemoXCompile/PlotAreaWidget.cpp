#include <QtCore/QDebug>
#include <QtCore/QRandomGenerator>
#include <QtMath>

#include "Theme.h"
#include "PlotAreaWidget.h"

namespace {
    constexpr int Indent_px = 20;
}

PlotAreaWidget::PlotAreaWidget(const QRect& rect, int xAxisTickCount, QWidget* parent)
    : QWidget(parent)
    , m_tickCount(xAxisTickCount)
    , m_xStep(0.0)
    , m_tick(0)
    , m_x(0.0)
    , m_lastPoint(0.0, 0.0)
    , m_line()
{
    // Don't erase previous paints.
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(false);
    setAttribute(Qt::WA_OpaquePaintEvent);

    // set the geometry
    setGeometry(rect);

    m_xStep = static_cast<qreal>(width()) / m_tickCount; // Calculate as real as need precision.

    qDebug() << "PlotAreaWidget()."
             << "m_tickCount =" << m_tickCount
             << "m_xStep = " << m_xStep;
    qDebug() << "..rect = " << rect;
    qDebug() << "..width() = " << width() << ", height() = " << height();
}

void PlotAreaWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(*m_pen);

    // Skip drawing 1st line that ends at x of 0
    if (m_line.x2() > 0.0)
    {
        // Cursor
        const int stepWidth = static_cast<int>(m_xStep);
        //painter.fillRect(m_x + 1, 0, stepWidth + stepWidth, height(), Qt::black);
        painter.fillRect(m_line.x1(), 0, stepWidth + stepWidth + stepWidth, height(), Theme::BackgroundColor);

        painter.drawLine(m_line);
    }

     //qDebug() << "paintEvent(). m_line = " << m_line << ", m_x = " << m_x;
}

void PlotAreaWidget::onAddValue(qreal v)
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

  //qDebug() << "PlotAreaWidget::onAddValue(). newPoint = " << newPoint << ", m_x = " << m_x;
}

int PlotAreaWidget::getTick()
{
    return m_tick;
}

int PlotAreaWidget::getTickCount()
{
    return m_tickCount;
}
