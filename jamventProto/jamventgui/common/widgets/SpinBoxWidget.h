#ifndef SPINBOXWIDGET_H
#define SPINBOXWIDGET_H

#include <QWidget>
#include <QSpinBox>
#include <QKeyEvent>

#include "SpinBoxWidget.h"

class SpinBoxWidget: public QSpinBox
{
    Q_OBJECT

  public:
    explicit SpinBoxWidget(QWidget *parent = nullptr);

  protected:
    void keyPressEvent(QKeyEvent *event) override;

  private:
};
#endif /* SPINBOXWIDGET_H */
