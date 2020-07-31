#include <QApplication>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QDebug>

#include "Theme.h"
#include "MainWidget.h"

namespace
{
}

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , m_stackedWidget(new QStackedWidget(this))
{
    // Define size and remove window chrome.
    resize(Theme::ScreenWidth_px, Theme::ScreenHeight_px);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->addWidget(m_stackedWidget);
    setLayout(layout);

    qDebug() << "m_stackedWidget->size() = " << m_stackedWidget->size();
}

MainWidget::~MainWidget()
{
}

void MainWidget::addWidget(Pages::Page_E page, QWidget *widget)
{
    m_pageMap.insert(page, widget);
    m_stackedWidget->addWidget(widget);
}

void MainWidget::showPage(Pages::Page_E page)
{
    const auto it = m_pageMap.find(page);
    if (it != m_pageMap.end())
    {
        QWidget *w = it.value();
        m_stackedWidget->setCurrentWidget(w);
        QWidget::show();
    }
    else
    {
        qDebug() << "Page Map with page =" << static_cast<int>(page) << "not found";
    }
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_C)
    {
        // Convert "c" key event to a tab keyevent to see if will change focus.
        QKeyEvent* e =  new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier, QString(QChar('\t')));
        QCoreApplication::sendEvent(this, e);
    }
    else if (event->key() == Qt::Key_Escape)
    {
        QApplication::instance()->quit();
    }
    else
    {
        QWidget::keyPressEvent(event);
    }
}
