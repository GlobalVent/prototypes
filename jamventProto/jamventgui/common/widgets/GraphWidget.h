#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QVector>
#include <QWidget>
#include <QLabel>

#include "VLabelWidget.h"

#include "PlotAreaWidget.h"

class GraphWidget: public QWidget
{
    Q_OBJECT
  public:
    using FloatVector = QVector<float>;

    struct InitParams
    {
        int xAxisTickCount;
        float xAxisMin;
        float xAxisMax;
        float yAxisMin;
        float yAxisMax;
        QString yAxisLabel;

        InitParams()
            : xAxisTickCount{0}, xAxisMin{0.0}, xAxisMax{0.0}, yAxisMin{0.0}, yAxisMax{0.0}, yAxisLabel{} {};
  };

    GraphWidget() = delete;
    GraphWidget(const InitParams &params, QWidget* parent);
    virtual ~GraphWidget() = default;

    int getTick();
    // Add and display a single value
    void onAddValue(float value);
    // Add and display a vector of values.
    // JPW \todo does not work. void onAddValues(const FloatVector& vector);

private:
   InitParams m_params;

   PlotAreaWidget* m_plotArea;
   VLabelWidget* m_yAxisLabel;
   QLabel *m_yAxisMaxLabel;
   QLabel *m_yAxisMinLabel;
};
#endif /* GRAPHWIDGET_H */
