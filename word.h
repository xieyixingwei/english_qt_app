#ifndef WORD_H
#define WORD_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QPair>

class word
{
public:
    struct paraphrase {
        QString mark;
        QList< QPair< QString, QString > > example;
    };

public:
    word();

private:
    QString m_word;
    QString m_soundmark;
    quint64 m_hot;
    quint64 m_timestamp;
    QStringList m_tag;
    QStringList m_sort;
    struct paraphrase m_paraphrase;
};

#endif // WORD_H
