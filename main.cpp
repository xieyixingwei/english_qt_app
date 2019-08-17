#include "mainwindow.h"
#include <QApplication>
#include "Setting.h"
#include <QVariant>
#include "Word.h"
#include "Sort.h"
#include "Classification.h"
#include "TextEdit.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

/*
    QStringList lines;
    lines << "- utility [ju'tɪləti] <hot:1 timestamp:1903221556>";
    lines << "    + n. 实用,公共设施,";
    lines << "        * utilities";
    lines << "        * [计] 实用程序";
    lines << "        * time";
    lines << "        * 时间";
    lines << "    + adj. 实用的,通用的";
    Word wd(lines);

    qDebug().noquote() << wd.ToRecordString(4);
    qDebug().noquote() << wd.ToDisplayString(4);

    //WordInterpretation iter(lines);
    //qDebug().noquote() << iter.ToDisplayString(0);
    //qDebug().noquote() << iter.ToRecordString(0);

    QList<WordInterpretation> iters = WordInterpretation::WordInterpretationList(lines);
    qDebug().noquote() << iters[0].ToDisplayString(0);
    qDebug().noquote() << iters[0].ToRecordString(0);

    qDebug().noquote() << iters[1].ToDisplayString(0);
    qDebug().noquote() << iters[1].ToRecordString(0);*/
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


    qDebug() << SETS.Value("Part_Of_Speech");
    qDebug() << SETS.GetGroup("SearchFiles");
    SETS.SetValue("Update_Hot", QVariant(true));
    qDebug() << SETS.Value("Update_Hot");
    SETS.RemoveGroup("Export");
    */

    return a.exec();
}
