/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QtCharts/QChart>
#include <QtCore/QTimer>
//#include <QLineSeries>
#include <QXYSeries>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE


class ChartWidget: public QChart
{
    Q_OBJECT
public:
    enum class ChartType {
        Line,
        Scatter,
        Spline
    };

    struct InitParams {
        ChartType type;
        QString yAxisTitle;
        int xAxisTickCount;
        int xAxisMinorTickCount;
        qreal xAxisMin;
        qreal xAxisMax;
        int yAxisTickCount;
        qreal yAxisMin;
        qreal yAxisMax;

        InitParams()
            : type{ChartType::Line}
            , yAxisTitle{}
            , xAxisTickCount{0}
            , xAxisMinorTickCount{1}
            , xAxisMin{0}
            , xAxisMax{0}
            , yAxisTickCount{0}
            , yAxisMin{0}
            , yAxisMax{0} {};
    };

    ChartWidget(const InitParams& params, QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = 0);
    virtual ~ChartWidget();

    void onTimeout();

private:
    QXYSeries *m_series;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;

    int m_indexCnt;
    qreal m_xStep;
    qreal m_x;
    qreal m_y;
};
#endif /* CHARTWIDGET_H */
