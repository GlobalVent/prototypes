#include <QApplication>
#include <QFontDatabase>

#include "Theme.h"
#include "MainWidget.h"

#include "JamCtrlSim.h"  // simulated version for now.

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

    JamCtrlSim jamCtrl;      // sim setup for now...

    // we probably need to direct to a ostream class when then goes to qdebug...
	//jamCtrlSim.setLogStream(qDebug());  
	jamCtrl.setTimeInterval(0.01);
	jamCtrl.init();		// ontime init, after we do any other settings.
	jamCtrl.runThread();

    loadFonts();

    MainWidget w;
    w.setJamCtlMgr(&jamCtrl);
    w.resize(Theme::ScreenWidth_px, Theme::ScreenHeight_px);
    
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    w.show();

    int rc;
    rc=a.exec();

    jamCtrl.killThread();
    return(rc);
}
