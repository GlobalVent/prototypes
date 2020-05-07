#ifndef PUSHBUTTONWIDGET_H
#define PUSHBUTTONWIDGET_H

#include <QPushButton>

class PushButtonWidget: public QPushButton
{
  Q_OBJECT

  public:
    PushButtonWidget(QWidget* parent = nullptr) = delete;
    PushButtonWidget(const QString& text, QWidget* parent = nullptr);

    ~PushButtonWidget() = default;

  private:

};
#endif /* PUSHBUTTONWIDGET_H */
