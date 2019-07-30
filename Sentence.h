#ifndef SENTENCE_H
#define SENTENCE_H

#include "Search.h"
#include <QPair>

class Sentence : public SearchResult
{
public:
    Sentence(const QStringList &lines);
    Sentence(const QString &a, const QString &b);
    Sentence();
    ~Sentence() = default;

    inline virtual void SetType(const QString &type) { m_type = type; }
    inline virtual QString GetType() { return m_type; }
    virtual void Display(QTextEdit *testedit);
    virtual void Update();
    inline virtual void SetPathfile(const QString &pathfile) { m_pathfile = pathfile; }
    inline virtual QString GetPathfile() { return m_pathfile; }

    inline QStringList GetPattern() { return m_pattern; }
    inline void AddPattern(const QString &pattern) { m_pattern << pattern; }
    inline void AddPattern(const QStringList &patterns) { m_pattern << patterns; }
    inline QStringList GetTense() { return m_tense; }
    inline void AddTense(const QString &tense) { m_tense << tense; }
    inline void AddTense(const QStringList &tenses) { m_tense << tenses; }
    void SetSentence(const QString &a, const QString &b);
    inline QPair<QString, QString> GetSentence() { return m_sentence; }
    inline void SetIndent(qint32 indent) { m_indent = indent; }
    inline void SetIndex(qint32 index) { m_index = index; }
    QString ToRecordString();
    QString ToDisplayString(qint32 index);
    void Record(const QString &pathfile = "");
    void Clear();

private:
    QString ExtractPatternTense(const QString &line);
    void Parse(const QStringList &lines);

private:
    QPair<QString, QString> m_sentence;
    QString m_pathfile;
    QString m_type;
    QStringList m_pattern;
    QStringList m_tense;
    qint32 m_indent;
    qint32 m_index;
};

#endif // SENTENCE_H
