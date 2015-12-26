#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("lightningmc");
    a.setApplicationDisplayName("Lightning");
    a.setApplicationVersion("0.3.1");
    a.setOrganizationName("GX Apps");
    MainWindow w;
    w.show();

    return a.exec();
}
