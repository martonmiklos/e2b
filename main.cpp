#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("MM");
    QCoreApplication::setApplicationName("EAGLE 2 Build");
    QCoreApplication::setApplicationVersion("1.0");
    MainWindow w;
    w.show();

    return a.exec();
}
