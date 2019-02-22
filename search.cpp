#include "search.h"
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QRegExp>

Search::Search()
{

}

Search::~Search()
{

}

QStringList Search::SearchWord(const QString &path, const QString &word)
{
    QStringList match;

    QFile file(path);

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        return match;
    }

    QTextStream text(&file);
    text.setCodec("UTF-8");

    QRegExp rex_word_start("^[ ]*-[ ]*" + word);
    QRegExp rex_word_end("^[ ]*-");

    while(!text.atEnd())
    {
        QString line = text.readLine();

        if(rex_word_start.exactMatch(line))
        {
            match << line;
        }
    }

    file.close();

    return match;
}
