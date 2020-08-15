#ifndef PUSHBUTTONWIDGET_H
#define PUSHBUTTONWIDGET_H

#include <QPushButton>
#include "FocusableWidget.h"

class PushButtonWidget : public QPushButton, public FocusableWidget
{
  Q_OBJECT

  public:
    PushButtonWidget(QWidget* parent = nullptr) = delete;
    PushButtonWidget(const QString& text, QWidget* parent = nullptr);

    ~PushButtonWidget() = default;

    virtual void setFocusState(FocusState state) override;

  protected:
    bool event(QEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

  private:

};
#endif /* PUSHBUTTONWIDGET_H */
