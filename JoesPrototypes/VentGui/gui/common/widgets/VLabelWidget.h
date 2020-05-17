#ifndef VLABELWIDGET_H
#define VLABELWIDGET_H

#include <QColor>
#include <QPainter>
#include <QLabel>


class VLabelWidget: public QLabel
{
    Q_OBJECT
public:
    VLabelWidget() = delete;
    VLabelWidget(const QRect& rect, QWidget* parent);
    virtual ~VLabelWidget() = default;

protected:
    void paintEvent(QPaintEvent *) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
};
#endif /* VLABELWIDGET_H */
