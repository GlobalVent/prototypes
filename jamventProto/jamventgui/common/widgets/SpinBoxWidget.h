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

  signals:
    void sigFocusChanged(bool hasFocus);

  protected:
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

  private:
};
#endif /* SPINBOXWIDGET_H */
