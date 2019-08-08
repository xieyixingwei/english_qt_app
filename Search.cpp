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

#define  APPEND(list, str)   do{ if(!(str).trimmed().isEmpty()){  (list) << (str); } }while(0)

QList<SearchResult *> Search::SearchTarget(const QStringList &filter, const QString &target)
{
    QStringList pathfiles = FindPathFileFromFilter(filter);

    QList<SearchResult *> searchResults;

    for(int i = 0; i < pathfiles.count(); i++)
    {
        searchResults += SearchTarget(pathfiles.at(i), target);
    }

    return searchResults;
}

QList<SearchResult *> Search::SearchTarget(const QString &pathfile, const QString &target)
{
    QList<SearchResult *> searchresults;

    TextEdit file(pathfile);

    for(int i = 0; i < file.Buf().count(); i++)
    {
        QString line = file.Buf().at(i);

        if(line.contains(QRegExp(".*" + target + ".*", Qt::CaseInsensitive))) // matching this word searched
        {
            QRegExp wordflag("^[ ]*-[ ]*.*");

            if(line.contains(QRegExp("^[ ]*-[ ]*" + target +".*", Qt::CaseInsensitive))) // it's a word of word dictionary
            {
                QStringList matchedlines;

                APPEND(matchedlines, line);

                while(++i < file.Buf().count())
                {
                    line = file.Buf().at(i);

                    if(line.contains(wordflag))
                    {
                        i--;
                        break;
                    }

                    APPEND(matchedlines, line);
                }

                SearchResult *pwd = new Word(matchedlines);
                pwd->SetPathfile(pathfile);
                searchresults.append(pwd);
            }
            else if(line.contains(QRegExp("^[ ]*\\+.*")) // it's in word dictionary
                    || line.contains(QRegExp("^[ ]*\\*.*")))
            {
                while(--i >= 0)
                {
                    line = file.Buf().at(i);

                    if(line.contains(wordflag))
                    {
                        break;
                    }
                }

                QStringList matchedlines;
                APPEND(matchedlines, line);

                while(++i < file.Buf().count())
                {
                    line = file.Buf().at(i);

                    if(line.contains(wordflag))
                    {
                        i--;
                        break;
                    }

                    APPEND(matchedlines, line);
                }

                SearchResult *pwd = new Word(matchedlines);
                pwd->SetPathfile(pathfile);
                searchresults.append(pwd);
            }
            else if(line.contains(QRegExp("^[ ]*-[ ]*.*" + target +".*", Qt::CaseInsensitive)))
            {
                ; // do nothing
            }
            else // it's in sentence
            {
                QStringList matchedlines;

                if(i > 0)
                {
                    APPEND(matchedlines, file.Buf().at(i - 1));
                }

                APPEND(matchedlines, line);

                if(++i < file.Buf().count())
                {
                    APPEND(matchedlines, file.Buf().at(i));
                }

                Sentence *pstc = new Sentence(matchedlines);
                pstc->SetPathfile(pathfile);
                searchresults.append(pstc);
            }
        }
    }

    return searchresults;
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
#if 0
void Search::FilterWordsAccordingTag(const QStringList &wordfiles, const QString &savefile, const QString &tag, int count)
{
    Sort<SortTimeStamp> sortvector;

    for(int k = 0; k < wordfiles.count(); k++)
    {
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
#endif
