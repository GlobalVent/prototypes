#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <QLabel>
#include "VLabelWidget.h"

#include "PlotAreaWidget.h"

class GraphWidget: public QWidget
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

    GraphWidget() = delete;
    GraphWidget(const InitParams &params, QWidget* parent);
    virtual ~GraphWidget() = default;

    void onAddValue(qreal v);
    void onTimeout();

protected:
    qreal getSinValue(int tick, int tickCount);
    qreal getRandValue();

private:
   InitParams m_params;

   PlotAreaWidget* m_plotArea;
   VLabelWidget* m_yAxisLabel;
   QLabel *m_yAxisMaxLabel;
};
#endif /* GRAPHWIDGET_H */
