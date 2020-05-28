#ifndef LABELEDDATAWIDGET_H
#define LABELEDDATAWIDGET_H

#include <QWidget>
#include <QLabel>

class LabeledDataWidget: public QWidget
{
  Q_OBJECT

  public:
    LabeledDataWidget(QWidget* parent = nullptr) = delete;
    LabeledDataWidget(const QString& label, QWidget* parent = nullptr);

    ~LabeledDataWidget() = default;

    QLabel* getDataLabel();

  private:
    QLabel* m_label = nullptr;
    QLabel* m_dataLabel = nullptr;
};
#endif /* LABELEDDATAWIDGET_H */
