#ifndef PLOTAREAWIDGET_H
#define PLOTAREAWIDGET_H

#include <QColor>
#include <QPainter>
#include <QTimer>
#include <Qt>
#include <QWidget>


class PlotAreaWidget: public QWidget
{
    Q_OBJECT
public:
  struct InitParams
  {
      int xAxisTickCount;
      qreal xAxisMin;
      qreal xAxisMax;
      qreal yAxisMin;
      qreal yAxisMax;

      InitParams()
          : xAxisTickCount{0}
          , xAxisMin{0.0}
          , xAxisMax{0.0}
          , yAxisMin{0.0}
          , yAxisMax{0.0} {};
  };
    PlotAreaWidget() = delete;
    PlotAreaWidget(const QRect& rect, int xAxisTickCount, QWidget* parent);
    virtual ~PlotAreaWidget() = default;

    void onAddValue(qreal v);
    int getTick();
    int getTickCount();

  protected:
    void paintEvent(QPaintEvent *) override;

private:
    QPen m_redPen{Qt::red, 2};
    QPen m_greenPen{Qt::green, 2};
    QPen* m_pen = &m_redPen;

    InitParams m_params;

    int m_tickCount;
    float m_xStep;  // Store as real for percision.
    int m_tick;

    qreal m_x;
    QPointF m_lastPoint;
    QLineF m_line;

};
#endif /* PLOTAREAWIDGET_H */
