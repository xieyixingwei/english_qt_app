#include "mainwindow.h"
#include <QApplication>
#include "Setting.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    qDebug() << SETS.FindKey("Sound_Volume");

    return a.exec();
}
