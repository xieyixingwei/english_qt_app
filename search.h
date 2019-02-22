#ifndef SEARCH_H
#define SEARCH_H

#include <QString>
#include <QStringList>

class Search
{
public:
    Search();
    virtual ~Search();

    virtual QStringList SearchWord(const QString &path, const QString &word);

protected:
    QString StrictSearch(const QString &content, const QString &word);
};

#endif // SEARCH_H
