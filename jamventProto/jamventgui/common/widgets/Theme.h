#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <Qt>
#include <QColor>
#include <QFont>

namespace Theme
{
    // Fonts
    static constexpr int SmallerFontSize_px = 11;
    static constexpr int SmallerFontSize_point = 11;
    static constexpr int MediumFontSize_px = 14;
    static constexpr int MediumFontSize_point = 14;
    static const QFont MainFont = QFont("Roboto", MediumFontSize_point);

    // Colors
    static const QColor BackgroundColor = QColor(Qt::white);
    static const QColor ForegroundColor = QColor(Qt::black);

    // Dimensions
    static constexpr int ScreenWidth_px = 800;
    static constexpr int ScreenHeight_px = 480;
}
#endif /* THEMEWIDGET_H */
