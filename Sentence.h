#ifndef SENTENCE_H
#define SENTENCE_H

#include "Search.h"
#include <QPair>

class Sentence : public SearchResult
{
public:
    Sentence(const QStringList &str);
    Sentence(const QString &a, const QString &b) { m_sentence = QPair<QString, QString>(a, b); }
    ~Sentence() = default;

    inline virtual void SetType(const QString &type) { m_type = type; }
    inline virtual QString GetType() { return m_type; }
    virtual void Display(QPlainTextEdit *testedit);
    virtual void Update();
    inline virtual void SetPathfile(const QString &pathfile) { m_pathfile = pathfile; }
    inline virtual QString GetPathfile() { return m_pathfile; }

    QString ToRecordString();
    QString ToDisplayString();
    void Record(const QString &pathfile = "");

private:
    QPair<QString, QString> m_sentence;
    QString m_pathfile;
    QString m_type;
};

#endif // SENTENCE_H
