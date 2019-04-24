#include "Search.h"
#include "TextEdit.h"
#include "Word.h"
#include "Sentence.h"
#include "Setting.h"
#include "DialogSet.h"

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
                pwd->SetType("word");
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

QList<Word *> Search::GetWordsOnTimeStamp(const QStringList &filter, int count)
{
    QStringList pathfiles = FindPathFileFromFilter(filter);

    QList<Word *> words;

    for(int i = 0; i < pathfiles.count(); i++)
    {
        TextEdit tedit(pathfiles[i]);

        QList<QStringList> results = tedit.FindAllBetween(QRegularExpression("^[ ]*-[ ]*.*"),
                                                          QRegularExpression("^[ ]*-[ ]*.*"));

        for(int i = 0; i < results.count(); i++)
        {
            Word *pwd = new Word(results[i]);
            if(pwd->GetHot().isEmpty())
            {
                delete pwd;
                continue;
            }

            if(words.count() < count)
            {
                pwd->SetPathfile(pathfiles[i]);
                pwd->SetType("word");
                words.append(pwd);
            }
            else
            {
                pwd->SetPathfile(pathfiles[i]);
                pwd->SetType("word");
                words.append(pwd);

                Word *earlier = words[0];
                for(i = 1; i < words.count(); i++)
                {
                    if(QDateTime::fromString(words[i]->GetTimeStamp(), "yyMMddhhmm") < QDateTime::fromString(earlier->GetTimeStamp(), "yyMMddhhmm"))
                    {
                        earlier = words[i];
                    }
                }

                words.removeOne(earlier);
                //delete earlier;
            }
        }
    }

    return words;
}

QList<Word *> Search::GetWordsOnHot(const QStringList &filter, int count)
{
    QStringList pathfiles = FindPathFileFromFilter(filter);

    QList<Word *> words;

    for(int k = 0; k < pathfiles.count(); k++)
    {
        TextEdit tedit(pathfiles[k]);

        QList<QStringList> results = tedit.FindAllBetween(QRegularExpression("^[ ]*-[ ]*.*"),
                                                          QRegularExpression("^[ ]*-[ ]*.*"));
        QStringList t;
        t << "- exercise ['ɛksɚsaɪz] <hot:10 timestamp:1901171729 tag:a,b,c sort:>";
        t << "+ n. 运动、练习、运用、操练、礼拜、典礼 ";
        t << "+ vt. 锻炼、练习、使用、使忙碌、使惊恐";
        t << "* Work-break Exercises ";
        t << "* 工间操 ";
        //qDebug() << results.count();
        quint64 len = sizeof(Word);
        qDebug() << len;
        for(int i = 0; i < 1000; i++)
        {
            /*
            QTime dieTime = QTime::currentTime().addMSecs(10); //10ms
            while( QTime::currentTime() < dieTime )
            {
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            }
*/


            //if(nullptr != pwd && pwd->GetHot().isEmpty())
            //{
                //delete pwd;
            //    continue;
            //}
/*
            pwd->SetPathfile(pathfiles[i]);
            pwd->SetType("word");
            words.append(pwd);

            if(words.count() > count)
            {
                Word *minhot = words[0];
                for(i = 1; i < words.count(); i++)
                {
                    if(words[i]->GetHot().toULong() < minhot->GetHot().toULong())
                    {
                        minhot = words[i];
                    }
                }

                words.removeOne(minhot);
                //delete minhot;
            } */
        }
    }

    return words;
}
