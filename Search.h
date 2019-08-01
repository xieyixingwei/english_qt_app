#ifndef SEARCH_H
#define SEARCH_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QPlainTextEdit>

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

    QList<Word *> GetWordsOnTimeStamp(const QStringList &filter, int count);
    QList<Word *> GetWordsOnHot(const QStringList &filter, int count);
};

#endif // SEARCH_H
