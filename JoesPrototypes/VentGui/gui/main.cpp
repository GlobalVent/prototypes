#include "MainWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if 1
    MainWidget w;
    w.resize(800, 480);
#else
    TreatmentWidget w;
#endif
    
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    w.show();
    return a.exec();
}
