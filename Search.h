#ifndef SEARCH_H
#define SEARCH_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QPlainTextEdit>

class SearchResult
{
public:
    SearchResult() = default;
    virtual ~SearchResult() = default;

    virtual void SetType(const QString &type) = 0;
    virtual QString GetType() = 0;
    virtual void Display(QTextEdit *testedit) = 0;
    virtual void Update() = 0;
    virtual void SetPathfile(const QString &pathfile) = 0;
    virtual QString GetPathfile() = 0;
};

class Search
{
public:
    Search() = default;
    virtual ~Search() = default;

    virtual QList<SearchResult *> SearchTarget(const QStringList &filter, const QString &target);
    virtual QList<SearchResult *> SearchTarget(const QString &pathfile, const QString &target);
};

#endif // SEARCH_H
