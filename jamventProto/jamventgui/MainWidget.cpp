#include <QHBoxLayout>
#include <QDebug>
#include "MainWidget.h"

namespace
{
}

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , m_stackedWidget(new QStackedWidget(this))
    , m_treatmentWidget(new TreatmentWidget(m_stackedWidget))
{

    QHBoxLayout* layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->addWidget(m_stackedWidget);
    setLayout(layout);

    m_stackedWidget->addWidget(m_treatmentWidget);
    m_stackedWidget->resize(m_treatmentWidget->size()); // Set to page size.

    qDebug() << "m_stackedWidget->size() = " << m_stackedWidget->size();
}

MainWidget::~MainWidget()
{
}

