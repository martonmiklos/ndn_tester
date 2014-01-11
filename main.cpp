#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //qputenv( "KDSOAP_DEBUG", "1" );
    QCoreApplication::setOrganizationName("MM");
    QCoreApplication::setApplicationName("NDN_test");
    QCoreApplication::setApplicationVersion("1.0");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
