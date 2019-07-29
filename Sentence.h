#ifndef SENTENCE_H
#define SENTENCE_H

#include "Search.h"
#include <QPair>

class Sentence : public SearchResult
{
public:
    Sentence(const QStringList &str);
    ~Sentence() = default;

    inline virtual void SetType(const QString &type) { m_type = type; }
    inline virtual QString GetType() { return m_type; }
    virtual void Display(QTextEdit *testedit);
    virtual void Update();
    inline virtual void SetPathfile(const QString &pathfile) { m_pathfile = pathfile; }
    inline virtual QString GetPathfile() { return m_pathfile; }

    inline QPair<QString, QString> GetSentence() { return m_sentence; }
    inline void SetIndent(qint32 indent) { m_indent = indent; }
    inline void SetIndex(qint32 index) { m_index = index; }
    QString ToRecordString();
    QString ToDisplayString(qint32 index);
    void Record(const QString &pathfile = "");

private:
    QString ExtractPatternTense(const QString &line);

private:
    QPair<QString, QString> m_sentence;
    QString m_pathfile;
    QString m_type;
    QString m_pattern;
    QString m_tense;
    qint32 m_indent;
    qint32 m_index;
};

#endif // SENTENCE_H
