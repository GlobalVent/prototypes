#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <Qt>
#include <QColor>
#include <QFont>

namespace Theme
{
    // Fonts
    static constexpr int FontSizeSmaller_px = 11;
    static constexpr int FontSizeSmaller_point = 11;
    static constexpr int FontSizeMedium_px = 14;
    static constexpr int FontSizeMedium_point = 14;
    static constexpr int FontSizeLarger_px = 16;
    static constexpr int FontSizeLarger_point = 16;
    static const QFont MainFont = QFont("Roboto", FontSizeMedium_point);

    // Colors
    static const QColor BackgroundColor = QColor(Qt::white);
    static const QColor ForegroundColor = QColor(Qt::black);

    // Dimensions
    static constexpr int ScreenWidth_px = 800;
    static constexpr int ScreenHeight_px = 480;
}
#endif /* THEMEWIDGET_H */
