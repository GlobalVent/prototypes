#include <QHash>
#include <QString>

#include "Theme.h"
#include "IeRatioSpinBoxWidget.h"

namespace
{
    // Not translating as these are ratios and should not be translated.
    constexpr const char *Ratio1to6Str = "1:6";
    constexpr const char *Ratio1to5Str = "1:5";
    constexpr const char *Ratio1to4Str = "1:4";
    constexpr const char *Ratio1to3Str = "1:3";
    constexpr const char *Ratio1to2Str = "1:2";
    constexpr const char *Ratio1to1Str = "1:1";
    constexpr const char *Ratio2to1Str = "2:1";
    constexpr const char *Ratio3to1Str = "3:1";
    constexpr const char *Ratio4to1Str = "4:1";
    constexpr const char *Ratio5to1Str = "5:1";
    constexpr const char *Ratio6to1Str = "6:1";

    QHash<IeRatioSpinBoxWidget::IeRatio_E, QString> IeRatioToStringHash =
        {
            {IeRatioSpinBoxWidget::Ratio1to6, Ratio1to6Str},
            {IeRatioSpinBoxWidget::Ratio1to5, Ratio1to5Str},
            {IeRatioSpinBoxWidget::Ratio1to4, Ratio1to4Str},
            {IeRatioSpinBoxWidget::Ratio1to3, Ratio1to3Str},
            {IeRatioSpinBoxWidget::Ratio1to2, Ratio1to2Str},
            {IeRatioSpinBoxWidget::Ratio1to1, Ratio1to1Str},
            {IeRatioSpinBoxWidget::Ratio2to1, Ratio2to1Str},
            {IeRatioSpinBoxWidget::Ratio3to1, Ratio3to1Str},
            {IeRatioSpinBoxWidget::Ratio4to1, Ratio4to1Str},
            {IeRatioSpinBoxWidget::Ratio5to1, Ratio5to1Str},
            {IeRatioSpinBoxWidget::Ratio6to1, Ratio6to1Str }
    };
}

IeRatioSpinBoxWidget::IeRatioSpinBoxWidget(QWidget* parent)
    : SpinBoxWidget(parent)
{
}

QString IeRatioSpinBoxWidget::textFromValue(int value) const
{
    IeRatio_E enumValue = static_cast<IeRatio_E>(value);
    return IeRatioToStringHash.value(enumValue);
}
