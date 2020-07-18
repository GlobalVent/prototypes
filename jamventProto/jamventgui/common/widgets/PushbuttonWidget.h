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

  protected:
    bool event(QEvent *event) override;

  private:

};
#endif /* PUSHBUTTONWIDGET_H */
