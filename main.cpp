#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("LightningMC");
    a.setApplicationVersion("0.3.1");
    a.setOrganizationName("GXA Software");
    MainWindow w;
    w.show();

    return a.exec();
}
