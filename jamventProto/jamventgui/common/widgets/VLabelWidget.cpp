#include <QtCore/QDebug>

#include "Theme.h"
#include "VLabelWidget.h"


VLabelWidget::VLabelWidget(const QRect& rect, QWidget* parent)
    : QLabel(parent)
{
    // set the geometry
    setGeometry(rect);
    setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    qDebug() << "VLabelWidget(). rect = " << rect;
}

void VLabelWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //qDebug() << "width =" << width() << "height =" << height() << ", text =" << text();

    QFont font = painter.font();
    font.setPointSize(Theme::SmallerFontSize_point);
    painter.setFont(font);
    const QRect textRect(0, 0, height(), width());

    painter.translate(0, height());
    painter.rotate(270.0);

    painter.drawText(textRect, text(), alignment());
}

QSize VLabelWidget::sizeHint() const
{
    QSize s = QLabel::sizeHint();
    return QSize(s.height(), s.width());
}

QSize VLabelWidget::minimumSizeHint() const
{
    QSize s = QLabel::minimumSizeHint();
    return QSize(s.height(), s.width());
}
