#ifndef LABELEDINPUTWIDGET_H
#define LABELEDINPUTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QKeyEvent>

#include "SpinBoxWidget.h"
#include "FocusableWidget.h"

class LabeledInputWidget: public QWidget, public FocusableWidget
{
  Q_OBJECT

  public:
    LabeledInputWidget(QWidget* parent = nullptr) = delete;
    LabeledInputWidget(const QString& label, QWidget* parent = nullptr);
    LabeledInputWidget(const QString &label, SpinBoxWidget* spinBox, QWidget* parent = nullptr);

    ~LabeledInputWidget() = default;

    QSpinBox* getSpinBox();

  protected:

  private:
    void init();

    QLabel* m_label = nullptr;
    QSpinBox* m_spinBox = nullptr;
};
#endif /* LABELEDINPUTWIDGET_H */
