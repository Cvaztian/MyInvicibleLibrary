#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //qDebug() << QT_VERSION_STR;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
