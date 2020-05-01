#ifndef REDGREENWIDGET_H
#define REDGREENWIDGET_H

#include <QColor>
#include <QPainter>
#include <QTimer>
#include <Qt>
#include <QWidget>


class RedGreenWidget: public QWidget
{
    Q_OBJECT
public:
  struct InitParams
  {
      int xAxisTickCount;

      InitParams()
          : xAxisTickCount{0} {};
  };

    RedGreenWidget() = delete;
    RedGreenWidget(const InitParams &params, QWidget* parent);
    virtual ~RedGreenWidget() = default;

    void onAddValue(qreal v);
    void onTimeout();

protected:
  void paintEvent(QPaintEvent *) override;
  qreal getSinValue();
  qreal getRandValue();

private:

   QPen m_redPen{Qt::red, 2};
   QPen m_greenPen{Qt::green, 2};
   QPen* m_pen = &m_redPen;

   int m_tickCount;
   float m_xStep;  // Store as real for percision.
   int m_tick;

   qreal m_x;
   QPointF m_lastPoint;
   QLineF m_line;

   //qreal m_y;
};
#endif /* REDGREENWIDGET_H */
