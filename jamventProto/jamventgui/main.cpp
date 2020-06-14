#include <QApplication>
#include <QFontDatabase>

#include "Theme.h"
#include "MainCtrl.h"

//#include "JamCtrlSim.h"  // simulated version for now.

void loadFonts()
{
    QFontDatabase::addApplicationFont(":/font/Roboto-Black.ttf");
    QFontDatabase::addApplicationFont(":/font/Roboto-BlackItalic.ttf");
    QFontDatabase::addApplicationFont(":/font/Roboto-Bold.ttf");
    QFontDatabase::addApplicationFont(":/font/Roboto-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/font/Roboto-Italic.ttf");
    QFontDatabase::addApplicationFont(":/font/Roboto-Light.ttf");
    QFontDatabase::addApplicationFont(":/font/Roboto-LightItalic.ttf");
    QFontDatabase::addApplicationFont(":/font/Roboto-Medium.ttf");
    QFontDatabase::addApplicationFont(":/font/Roboto-MediumItalic.ttf");
    QFontDatabase::addApplicationFont(":/font/Roboto-Regular.ttf");
    QFontDatabase::addApplicationFont(":/font/Roboto-Thin.ttf");
    QFontDatabase::addApplicationFont(":/font/Roboto-ThinItalic.ttf");
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // loadFonts();

    MainCtrl ctl;   // Instantiate the main controller to controll the GUI pages.
    int rc;
    rc=a.exec();

    return(rc);
}
