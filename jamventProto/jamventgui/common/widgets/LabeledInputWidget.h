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
    LabeledInputWidget(QWidget *parent = nullptr) = delete;
    LabeledInputWidget(const QString& label);
    LabeledInputWidget(const QString &label, SpinBoxWidget* spinBox);

    ~LabeledInputWidget() = default;

    SpinBoxWidget* getSpinBox();

    virtual void setFocusState(FocusState state) override;

  protected:
    void paintEvent(QPaintEvent *p_event) override;
    //void focusInEvent(QFocusEvent *event) override;
    //void focusOutEvent(QFocusEvent *event) override;

  private:
    void init();

    void onSpinBoxFocusChanged(bool hasFocus);

    QLabel* m_label = nullptr;
    SpinBoxWidget* m_spinBox = nullptr;
};
#endif /* LABELEDINPUTWIDGET_H */
