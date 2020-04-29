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
    RedGreenWidget() = delete;
    RedGreenWidget(QWidget* parent);
    virtual ~RedGreenWidget() = default;

    void onAddValue(qreal v);
    void onTimeout();

protected:
  void paintEvent(QPaintEvent *) override;

private:
   qreal getSinValue();
   qreal getRandValue();

   QPen m_redPen{Qt::red, 2};
   QPen m_greenPen{Qt::green, 2};
   QPen* m_pen = &m_redPen;

   int m_tickCount;
   int m_xStep;
   int m_tick;

   qreal m_x;
   QPointF m_lastPoint;
   QLineF m_line;

   int m_timerInterval_ms;
   QTimer m_timer;

   //qreal m_y;
};
#endif /* REDGREENWIDGET_H */
