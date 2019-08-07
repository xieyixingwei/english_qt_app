#include "mainwindow.h"
#include <QApplication>
#include "Setting.h"
#include <QVariant>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
/*
    qDebug() << SETS[KEY_SENTENCE_PATTERN].toString();
    qDebug() << SETS[KEY_PART_OF_SPEECH].toString();
    qDebug() << SETS[KEY_SENTENCE_TENSE].toString();
    qDebug() << SETS[KEY_WORD_TAGS].toString();
    qDebug() << SETS[KEY_AUTO_ADD_WORD].toBool();
    qDebug() << SETS[KEY_SENTENCE_FILE].toString();
    qDebug() << SETS[KEY_SOUND_DIR].toString();
    qDebug() << SETS[KEY_SOUND_VOLUME].toInt();
    qDebug() << SETS[KEY_UNNOTE_WORD_FILE].toString();
    qDebug() << SETS[KEY_UPDATE_HOT].toBool();
    qDebug() << SETS[KEY_UPDATE_TIMESTAMP].toBool();

    qDebug() << SETS[KEY_EXPORT_FILE_ON_HOT].toString();
    qDebug() << SETS[KEY_EXPORT_FILE_ON_TIMESTAMP].toString();
    qDebug() << SETS[KEY_EXPORT_FILE_ON_TIMESTAMP_SCOPE].toString();
    qDebug() << Settings::ToStringList(SETS.GetGroup(GROUP_SEARCH_FILES));
    */
    /*
    qDebug() << SETS.Value("Part_Of_Speech");
    qDebug() << SETS.GetGroup("SearchFiles");
    SETS.SetValue("Update_Hot", QVariant(true));
    qDebug() << SETS.Value("Update_Hot");
    SETS.RemoveGroup("Export");
    */

    return a.exec();
}
