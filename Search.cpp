#include "Search.h"
#include "TextEdit.h"
#include "Word.h"
#include "Sentence.h"
#include "Setting.h"
#include "DialogSet.h"
#include "Sort.h"

#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QRegExp>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QRegularExpression>
#include <QCoreApplication>


QList<SearchResult *> Search::SearchTarget(const QStringList &filter, const QString &keyword)
{
    QStringList pathfiles = FindPathFileFromFilter(filter);

    QList<SearchResult *> results;

    for(int i = 0; i < pathfiles.count(); i++)
    {
        QFileInfo fileinfo(pathfiles[i]);
        if(fileinfo.fileName().contains("word"))
        {
            results.append(SearchInWords(pathfiles[i], keyword));
        }
        else if(fileinfo.fileName().contains("sentence"))
        {
            results.append(SearchInSentences(pathfiles[i], keyword));
        }
        else
        {
            results.append(SearchInEssays(pathfiles[i], keyword));
        }
    }

    return results;
}

QStringList Search::FindPathFileFromFilter(const QStringList &filter)
{
    QStringList pathfiles;

    for(int i = 0; i < filter.count(); i++)
    {
        QFileInfo fileinfo(filter[i]);
        QDir dir(fileinfo.path());

        QFileInfoList list = dir.entryInfoList(QStringList(fileinfo.fileName()));

        for(int k = 0; k < list.count(); k++)
        {
            if(list[k].isFile() && !pathfiles.contains(list[k].filePath()))
            {
                pathfiles << list[k].filePath();
            }
        }
    }

    return pathfiles;
}

void Search::FilterWordsAccordingHot(const QStringList &wordfiles, const QString &savefile, int count)
{
    Sort<SortHot> sortvector;

    for(int k = 0; k < wordfiles.count(); k++)
    {
        TextEdit text(wordfiles[k]);

        for(int i = 0; i < text.Buf().count(); i++)
        {
            QStringList wordTexts = text.Find(QRegularExpression("^[ ]*-.*"), QRegularExpression("^[ ]*-.*"));
            if(!wordTexts.isEmpty())
            {
                sortvector.Append(SortHot(wordTexts.join("\n")));
            }
            else
            {
                break;
            }
        }
    }

    TextEdit text(savefile, QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    int i = 0;
    QVector<SortHot> *vector = sortvector.SortItems();

    for(QVector<SortHot>::iterator it = vector->begin();
        it != vector->end(); it++)
    {
        text << (*it).Text();
        if(++i == count)
        {
            break;
        }
    }
}


void Search::FilterWordsAccordingTimeStamp(const QStringList &wordfiles, const QString &savefile, int count)
{
    Sort<SortTimeStamp> sortvector;

    for(int k = 0; k < wordfiles.count(); k++)
    {
        QFileInfo fileinfo(wordfiles[k]);
        if(!fileinfo.fileName().contains("word"))
        {
            continue;
        }

        TextEdit text(wordfiles[k]);

        while(1)
        {
            QStringList wordTexts = text.Find(QRegularExpression("^[ ]*-.*"), QRegularExpression("^[ ]*-.*"));
            if(!wordTexts.isEmpty())
            {
                sortvector.Append(SortTimeStamp(wordTexts.join("\n")));
            }
            else
            {
                break;
            }
        }
    }

    TextEdit text(savefile, QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    int i = 0;
    QVector<SortTimeStamp> *vector = sortvector.SortItems();

    for(QVector<SortTimeStamp>::iterator it = vector->begin();
        it != vector->end(); it++)
    {
        text << (*it).Text();
        if(++i == count)
        {
            break;
        }
    }
}

void Search::FilterWordsAccordingTimeStamp(const QStringList &wordfiles, const QString &savefile, const QDateTime &begin, const QDateTime &end)
{
    Sort<SortTimeStamp> sortvector;

    for(int k = 0; k < wordfiles.count(); k++)
    {
        QFileInfo fileinfo(wordfiles[k]);
        if(!fileinfo.fileName().contains("word"))
        {
            continue;
        }

        TextEdit text(wordfiles[k]);

        while(1)
        {
            QStringList wordTexts = text.Find(QRegularExpression("^[ ]*-.*"), QRegularExpression("^[ ]*-.*"));
            if(!wordTexts.isEmpty())
            {
                sortvector.Append(SortTimeStamp(wordTexts.join("\n")));
            }
            else
            {
                break;
            }
        }
    }

    TextEdit text(savefile, QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    QVector<SortTimeStamp> *vector = sortvector.SortItems();

    for(QVector<SortTimeStamp>::iterator it = vector->begin();
        it != vector->end(); it++)
    {
        if((*it).Value() > begin.toSecsSinceEpoch() && (*it).Value() < end.toSecsSinceEpoch())
        {
            text << (*it).Text();
        }
    }
}

void Search::FilterWordsAccordingTag(const QStringList &wordfiles, const QString &savefile, const QString &tag)
{
    Sort<SortTimeStamp> sortvector;

    for(int k = 0; k < wordfiles.count(); k++)
    {
        QFileInfo fileinfo(wordfiles[k]);
        if(!fileinfo.fileName().contains("word"))
        {
            continue;
        }

        TextEdit text(wordfiles[k]);

        while(1)
        {
            QStringList wordTexts = text.Find(QRegularExpression("^[ ]*-.*"), QRegularExpression("^[ ]*-.*"));
            if(wordTexts.isEmpty())
            {
                break;
            }

            if(wordTexts[0].contains(tag))
            {
                sortvector.Append(SortTimeStamp(wordTexts.join("\n")));
            }
        }
    }

    TextEdit text(savefile, QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    QVector<SortTimeStamp> *vector = sortvector.SortItems();
    for(QVector<SortTimeStamp>::iterator it = vector->begin();
        it != vector->end(); it++)
    {
        text << (*it).Text();
    }
}

QList<SearchResult*> Search::SearchInWords(const QString &wordfile, const QString &keyword)
{
    QList<SearchResult *> words;

    TextEdit text(wordfile);

    while(1)
    {
        QStringList wordTexts = text.Find(QRegularExpression("^[ ]*-.*"), QRegularExpression("^[ ]*-.*"));
        if(wordTexts.isEmpty())
        {
            break;
        }

        if(wordTexts.join(" ").remove(QRegularExpression("(?<=<).*?(?=>)")).contains(keyword))
        {
            Word *pwd = new Word(wordTexts);
            pwd->SetPathfile(wordfile);
            words.append(pwd);
        }
    }

    return words;
}

QList<SearchResult *> Search::SearchInSentences(const QString &sentencefile, const QString &keyword)
{
    QList<SearchResult *> sentences;
    TextEdit text(sentencefile);

    for(int i = 0; i < text.Buf().count(); i++)
    {
        if(text.Buf()[i].contains(keyword))
        {
            if(i > 0 && !text.Buf()[i - 1].trimmed().isEmpty())
            {
                sentences.append(new Sentence(text.Buf()[i - 1], text.Buf()[i]));
            }
            else if(i + 1 <  text.Buf().count() && !text.Buf()[i + 1].trimmed().isEmpty())
            {
                sentences.append(new Sentence(text.Buf()[i], text.Buf()[i + 1]));
            }
            else
            {
                sentences.append(new Sentence(text.Buf()[i], ""));
            }
        }
    }

    return sentences;
}

QList<SearchResult *> Search::SearchInEssays(const QString &essayfile, const QString &keyword)
{
    QList<SearchResult *> results;
    TextEdit text(essayfile);

    for(int i = 0; i < text.Buf().count(); i++)
    {
        if(text.Buf()[i].contains(keyword))
        {
            results.append(new Sentence(text.Buf()[i], ""));
        }
    }

    return results;
}



void Search::FilterSentencesAccordingTimeStamp(const QStringList &files, const QString &savefile, int count)
{
    Sort<SortTimeStamp> sortvector;

    for(int k = 0; k < files.count(); k++)
    {
        QFileInfo fileinfo(files[k]);
        if(!fileinfo.fileName().contains("sentence"))
        {
            continue;
        }

        TextEdit text(files[k]);
        while(1)
        {
            QStringList sentenceTexts = text.FindSentences();
            if(sentenceTexts.isEmpty())
            {
                break;
            }

            sortvector.Append(SortTimeStamp(sentenceTexts.join("\n") + "\n"));
        }
    }

    TextEdit text(savefile, QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    int i = 0;
    QVector<SortTimeStamp> *vector = sortvector.SortItems();

    for(QVector<SortTimeStamp>::iterator it = vector->begin();
        it != vector->end(); it++)
    {
        text << (*it).Text();
        if(++i == count)
        {
            break;
        }
    }
}

void Search::FilterSentencesAccordingTimeStamp(const QStringList &files, const QString &savefile, const QDateTime &begin, const QDateTime &end)
{
    Sort<SortTimeStamp> sortvector;

    for(int k = 0; k < files.count(); k++)
    {
        QFileInfo fileinfo(files[k]);
        if(!fileinfo.fileName().contains("sentence"))
        {
            continue;
        }

        TextEdit text(files[k]);
        while(1)
        {
            QStringList sentenceTexts = text.FindSentences();
            if(sentenceTexts.isEmpty())
            {
                break;
            }

            sortvector.Append(SortTimeStamp(sentenceTexts.join("\n") + "\n"));
        }
    }

    TextEdit text(savefile, QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    QVector<SortTimeStamp> *vector = sortvector.SortItems();

    for(QVector<SortTimeStamp>::iterator it = vector->begin();
        it != vector->end(); it++)
    {
        if((*it).Value() > begin.toSecsSinceEpoch() && (*it).Value() < end.toSecsSinceEpoch())
        {
            text << (*it).Text();
        }
    }
}

void Search::FilterSentencesAccordingTag(const QStringList &files, const QString &savefile, const QString &tag)
{
    Sort<SortTimeStamp> sortvector;

    for(int k = 0; k < files.count(); k++)
    {
        QFileInfo fileinfo(files[k]);
        if(!fileinfo.fileName().contains("sentence"))
        {
            continue;
        }

        TextEdit text(files[k]);

        while(1)
        {
            QStringList sentenceTexts = text.FindSentences();
            if(sentenceTexts.isEmpty())
            {
                break;
            }

            if(sentenceTexts.join("\n").contains(tag))
            {
                sortvector.Append(SortTimeStamp(sentenceTexts.join("\n") + "\n"));
            }
        }
    }

    TextEdit text(savefile, QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
    QVector<SortTimeStamp> *vector = sortvector.SortItems();
    for(QVector<SortTimeStamp>::iterator it = vector->begin();
        it != vector->end(); it++)
    {
        text << (*it).Text();
    }
}
