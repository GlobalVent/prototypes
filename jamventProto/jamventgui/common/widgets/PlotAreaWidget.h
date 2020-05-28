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
    static constexpr float YMin   = -1.0;
    static constexpr float YMax   = 1.0;
    static constexpr float YRange = YMax - YMin;
    
    struct InitParams
    {
        int xAxisTickCount;
        qreal xAxisMin;
        qreal xAxisMax;
        qreal yAxisMin;
        qreal yAxisMax;

        InitParams()
            : xAxisTickCount{0}, xAxisMin{0.0}, xAxisMax{0.0}, yAxisMin{0.0}, yAxisMax{0.0} {};
  };
    PlotAreaWidget() = delete;
    PlotAreaWidget(const QRect& rect, int xAxisTickCount, QWidget* parent);
    virtual ~PlotAreaWidget() = default;

    // \param[in] v - Value to plot, -1.0 <= v <= 1.0;
    void onAddValue(float v);

    int getTick();
    int getTickCount();

  protected:
    void paintEvent(QPaintEvent *) override;

private:
    QPen m_redPen{Qt::red, 2};
    QPen m_greenPen{Qt::green, 2};
    QPen* m_pen = &m_redPen;

    QColor m_bgColor;

    InitParams m_params;

    int m_tickCount;
    float m_xStep;  // Store as real for precision.
    int m_tick;

    float m_x;
    QPointF m_lastPoint;
    QLineF m_line;

};
#endif /* PLOTAREAWIDGET_H */
