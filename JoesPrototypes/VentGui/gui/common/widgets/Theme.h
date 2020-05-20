#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <Qt>
#include <QColor>
#include <QFont>

namespace Theme
{
    // Fonts
    static constexpr int MainFontSize_px = 12;
    static constexpr int MainFontSize_point = 12;
    static const QFont MainFont = QFont("Roboto", MainFontSize_point);
    static constexpr int YAxisLabelFontSize_px = 11;
    // Colors
    static const QColor BackgroundColor = QColor(Qt::white);
    static const QColor ForegroundColor = QColor(Qt::black);
    // Dimensions
    static constexpr int ScreenWidth_px = 800;
    static constexpr int ScreenHeight_px = 480;
}
#endif /* THEMEWIDGET_H */
