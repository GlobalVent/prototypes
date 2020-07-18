#ifndef IERATIOSPINBOXWIDGET_H
#define IERATIOSPINBOXWIDGET_H

#include "SpinBoxWidget.h"


class IeRatioSpinBoxWidget: public SpinBoxWidget
{
  Q_OBJECT

  public:

    // Original range was from 1:6 - 6:1.  Now using 1:4 - 4:1.
    enum IeRatio_E
    {
        Ratio1to6 = 0,
        Ratio1to5,
        Ratio1to4,
        RatioMin = Ratio1to4,
        Ratio1to3,
        Ratio1to2,
        Ratio1to1,
        Ratio2to1,
        Ratio3to1,
        Ratio4to1,
        RatioMax = Ratio4to1,
        Ratio5to1,
        Ratio6to1
    };

    explicit IeRatioSpinBoxWidget(QWidget* parent = nullptr);
    ~IeRatioSpinBoxWidget() = default;

    QString textFromValue(int value) const override;

  private:
};
#endif /* IERATIOSPINBOXWIDGET_H */
