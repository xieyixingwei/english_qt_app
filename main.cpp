#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
/*
    QRegularExpression rex(QString("(?<=\\[).*?(?=\\])"));
    QString line("Native compilation [pattern: S + O, tense: was/were + Ving]");
    QRegularExpressionMatch match = rex.match(line);
    if(match.hasMatch())
    {
        qDebug() <<  match.captured(0);
        qDebug() <<  match.captured(1);
        qDebug() <<  match.captured(2);
    }
*/
    return a.exec();
}
