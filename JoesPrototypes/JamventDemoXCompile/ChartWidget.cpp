
#include "ChartWidget.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QRandomGenerator>
#include <QtCore/QDebug>

ChartWidget::ChartWidget(const InitParams& params, QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    m_series(nullptr),
    m_axisX(new QValueAxis()),
    m_axisY(new QValueAxis()),
    m_step(0),
    m_x(0.0),
    m_y(0.0)
{
    legend()->hide();
	//setAnimationOptions(QChart::AllAnimations);
	setAnimationOptions(QChart::NoAnimation);
    setMargins(QMargins(0,0,0,0));
    setBackgroundRoundness(0);
    setBackgroundVisible(false);

    QObject::connect(&m_timer, &QTimer::timeout, this, &ChartWidget::handleTimeout);
    m_timer.setInterval(params.timerInterval_ms);

    if (ChartType::Scatter == params.type)
    {
        QScatterSeries* scatterSeries = new QScatterSeries(this);
        scatterSeries->setMarkerSize(6.0);
        m_series = scatterSeries;
    }
    else if (ChartType::Spline == params.type)
    {
        m_series = new QSplineSeries(this);
    }
    else
    {
        m_series = new QLineSeries(this);
    }
    QPen green(Qt::green);
    green.setWidth(2);
    m_series->setPen(green);
    m_series->append(m_x, m_y);

    addSeries(m_series);
	
	constexpr int LabelPixelSize = 12;
	
	QFont font = m_axisX->labelsFont();
	font.setPixelSize(LabelPixelSize);
	qDebug() << "font.pixelSize() = " << font.pixelSize();

    addAxis(m_axisX, Qt::AlignBottom);
    addAxis(m_axisY, Qt::AlignLeft);
	
	m_axisX->setLabelsFont(font);
	m_axisY->setLabelsFont(font);

    m_series->attachAxis(m_axisX);
    m_series->attachAxis(m_axisY);
    m_axisX->setTickCount(params.xAxisTickCount);
    m_axisX->setRange(params.xAxisMin, params.xAxisMax);

    m_axisY->setTickCount(params.yAxisTickCount);
    m_axisY->setRange(params.yAxisMin, params.yAxisMax);
	m_axisY->setTitleFont(font);
    m_axisY->setTitleText(params.yAxisTitle);

    m_timer.start();
}

ChartWidget::~ChartWidget()
{

}

void ChartWidget::handleTimeout()
{
	static int count = 100;
	qDebug() << "Chart::handleTimeout() called. count = " << count;
	
    qreal xPixel = plotArea().width() / (m_axisX->tickCount() - 1);
    qreal xValue = (m_axisX->max() - m_axisX->min()) / (m_axisX->tickCount() - 1);
    qreal yPixel = plotArea().height() / (m_axisY->tickCount() - 1);
    qreal yValue = (m_axisY->max() - m_axisY->min()) / (m_axisY->tickCount() - 1);
    qreal yRange = m_axisY->max() - m_axisY->min();

    m_x += xValue;
    double rand = QRandomGenerator::global()->bounded(yRange);
    m_y = rand + m_axisY->min();

    qDebug() << "plotArea().width() = " << plotArea().width() << ", m_axisX->tickCount() = " << m_axisX->tickCount();
    qDebug() << "m_axisX->max() = " << m_axisX->max() << ", m_axisX->min() = " << m_axisX->min();
    qDebug() << "xPixel = " << xPixel << ", xValue = " << xValue;

    qDebug() << "m_axisY->max() = " << m_axisY->max() << ", m_axisY->min() = " << m_axisY->min();
    qDebug() << "yPixel = " << yPixel << ", yValue = " << yValue;
    qDebug() << "yRange = " << yRange;

    qDebug() << "m_x = " << m_x << ", m_y = " << m_y;

    if (m_x >= m_axisX->tickCount())
    {
	    // At end of graph. Scroll so stays in view
	    //scroll(xPixel, 0);
	    //m_series->remove(0); // Remove point that scrolled off the screen.
		// Start back at left.
		m_series->clear();
	    m_x = 0.0;
    }
	m_series->append(m_x, m_y);
	
	// Stop when count reaches 0.
	if(count <= 0)
	{
		m_timer.stop();
	}
	
	count--;
}
