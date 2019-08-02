#ifndef SEARCH_H
#define SEARCH_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QPlainTextEdit>
#include <QDateTime>

class SearchResult
{
public:
    using E_ResultType = enum {
        E_TYPE_WORD = 0,
        E_TYPE_SENTENCE,
    };

public:
    SearchResult() = default;
    virtual ~SearchResult() = default;

    virtual E_ResultType Type() = 0;
    virtual void Display(QTextEdit *testedit) = 0;
    virtual void Update() = 0;
    virtual void SetPathfile(const QString &pathfile) = 0;
    virtual QString GetPathfile() = 0;
};

class Word;

class Search
{
public:
    Search() = default;
    virtual ~Search() = default;

    virtual QList<SearchResult *> SearchTarget(const QStringList &filter, const QString &target);
    virtual QList<SearchResult *> SearchTarget(const QString &pathfile, const QString &target);
    QStringList FindPathFileFromFilter(const QStringList &filter);

    void FilterWordsAccordingTimeStamp(const QStringList &wordfiles, const QString &savefile, int count);
    void FilterWordsAccordingTimeStamp(const QStringList &wordfiles, const QString &savefile, const QDateTime &begin, const QDateTime &end);
    void FilterWordsAccordingHot(const QStringList &wordfiles, const QString &savefile, int count);
};

#endif // SEARCH_H
