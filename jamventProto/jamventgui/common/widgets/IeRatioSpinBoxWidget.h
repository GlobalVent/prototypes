#ifndef IERATIOSPINBOXWIDGET_H
#define IERATIOSPINBOXWIDGET_H

#include <QSpinBox>


class IeRatioSpinBoxWidget: public QSpinBox
{
  Q_OBJECT

  public:

    enum IeRatio_E
    {
        RatioMin = 0,
        Ratio1to6 = RatioMin,
        Ratio1to5,
        Ratio1to4,
        Ratio1to3,
        Ratio1to2,
        Ratio1to1,
        Ratio2to1,
        Ratio3to1,
        Ratio4to1,
        Ratio5to1,
        Ratio6to1,
        RatioMax = Ratio6to1
    };

    explicit IeRatioSpinBoxWidget(QWidget* parent = nullptr);
    ~IeRatioSpinBoxWidget() = default;

    QString textFromValue(int value) const override;

  private:
};
#endif /* IERATIOSPINBOXWIDGET_H */
