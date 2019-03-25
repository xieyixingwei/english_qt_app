#include "Search.h"
#include "TextEdit.h"
#include "Word.h"
#include "Sentence.h"

#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QRegExp>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>

#define  APPEND(list, str)   do{ if(!(str).trimmed().isEmpty()){  (list) << (str); } }while(0)

QList<SearchResult *> Search::SearchTarget(const QStringList &filter, const QString &target)
{
    QStringList *pathfile = new QStringList;

    for(int i = 0; i < filter.count(); i++)
    {
        QFileInfo fileinfo(filter[i]);
        QDir dir(fileinfo.path());

        QFileInfoList list = dir.entryInfoList(QStringList(fileinfo.fileName()));

        for(int k = 0; k < list.count(); k++)
        {
            if(list[k].isFile() && !pathfile->contains(list[k].filePath()))
            {
                pathfile->append(list[k].filePath());
            }
        }
    }

    QList<SearchResult *> SearchResults;

    for(int i = 0; i < pathfile->count(); i++)
    {
        SearchResults += SearchTarget(pathfile->at(i), target);
    }

    return SearchResults;
}

QList<SearchResult *> Search::SearchTarget(const QString &pathfile, const QString &target)
{
    QList<SearchResult *> searchresults;

    QFile file(pathfile);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return searchresults;
    }

    QTextStream text(&file);
    text.setCodec("UTF-8");

    QStringList *filebuf = new QStringList;

    while(!text.atEnd())
    {
        *filebuf << text.readLine();
    }

    file.close();

    for(int i = 0; i < filebuf->count(); i++)
    {
        QString line = filebuf->at(i);

        if(line.contains(QRegExp(".*" + target + ".*", Qt::CaseInsensitive))) // matching this word searched
        {
            QRegExp wordflag("^[ ]*-[ ]*.*");

            if(line.contains(QRegExp("^[ ]*-[ ]*" + target +".*", Qt::CaseInsensitive))) // it's a word of word dictionary
            {
                QStringList matchedlines;

                APPEND(matchedlines, line);

                while(++i < filebuf->count())
                {
                    line = filebuf->at(i);

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
                    line = filebuf->at(i);

                    if(line.contains(wordflag))
                    {
                        break;
                    }
                }

                QStringList matchedlines;
                APPEND(matchedlines, line);

                while(++i < filebuf->count())
                {
                    line = filebuf->at(i);

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
                    APPEND(matchedlines, filebuf->at(i - 1));
                }

                APPEND(matchedlines, line);

                if(++i < filebuf->count())
                {
                    APPEND(matchedlines, filebuf->at(i));
                }

                Sentence *pstc = new Sentence(matchedlines);
                pstc->SetPathfile(pathfile);
                searchresults.append(pstc);
            }
        }
    }

    delete filebuf;
    return searchresults;
}
