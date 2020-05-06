#ifndef LABELEDINPUTWIDGET_H
#define LABELEDINPUTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>


class LabeledInputWidget: public QWidget
{
  Q_OBJECT

  public:
    LabeledInputWidget(QWidget* parent = nullptr) = delete;
    LabeledInputWidget(const QString& label, QWidget* parent = nullptr);

    ~LabeledInputWidget() = default;

    QSpinBox* getSpinBox();

  private:
    QLabel* m_label = nullptr;
    QSpinBox* m_spinBox = nullptr;
};
#endif /* LABELEDINPUTWIDGET_H */
