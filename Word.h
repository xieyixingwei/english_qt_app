#ifndef WORD_H
#define WORD_H

#include "Search.h"
#include "Sentence.h"

#include <QString>
#include <QStringList>
#include <QList>
#include <QPair>
#include <QMediaPlayer>

class WordInterpretation
{
public:
    WordInterpretation(const QStringList &lines) { Parse(lines); }
    WordInterpretation() = default;
    ~WordInterpretation() = default;

    inline void SetPos(const QString &pos) { m_pos = pos; }
    inline QString GetPos() const { return m_pos; }
    inline void AddMean(const QString &mean) { m_means << mean; }
    inline void AddMean(const QStringList &means) { m_means << means; }
    inline void SetMean(const QStringList &mean) { m_means = mean; }
    inline QStringList GetMean() const { return m_means; }

    inline void AddExample(const Sentence &exp) { m_examples << exp; }
    inline void AddExample(const QList<Sentence> &exps) { m_examples << exps; }
    inline QList<Sentence> GetExample() const { return m_examples; }

    QString ToRecordString(qint32 indent);
    QString ToDisplayString(qint32 indent);
    void Clear();
    static QList<WordInterpretation> WordInterpretationList(const QStringList &lines);

private:
    void Parse(const QStringList &lines);

private:
    QString m_pos;
    QStringList m_means;
    QList<Sentence> m_examples;
};

class Word : public SearchResult
{
public:
    Word(const QStringList &lines) : m_mediaplayer(new QMediaPlayer) { Parse(lines); }
    Word(const Word & obj);
    Word() { }
    ~Word() = default;

    virtual E_ResultType Type() { return E_TYPE_WORD; }
    virtual void Display(QTextEdit *testedit);
    virtual void Update();
    inline virtual void SetPathfile(const QString &pathfile) { m_pathfile = pathfile; }
    inline virtual QString GetPathfile() { return m_pathfile; }

    inline QString GetWord() const { return m_word; }
    inline void SetWord(const QString &word) { m_word = word; }

    inline QString GetSoundMark() const { return m_soundmark; }
    inline void SetSoundMark(const QString &soundmark) { m_soundmark = soundmark; }

    inline QString GetHot() const { return m_hot; }
    inline void SetHot(const QString &hot) { m_hot = hot; }

    inline QString GetTimeStamp() const { return m_timestamp; }
    inline void SetTimeStamp(const QString &timestamp) { m_timestamp = timestamp; }

    inline QStringList GetTags() const { return m_tags; }
    inline void SetTag(const QStringList &tags) { m_tags = tags; }
    inline void AddTag(const QString &tag) { m_tags << tag; }
    inline void AddTag(const QStringList &tags) { m_tags << tags; }

    inline QList<WordInterpretation> GetInerpretion() { return m_interpretations; }
    inline void SetInerpretion(const QList<WordInterpretation> &itps) { m_interpretations = itps; }
    inline void AddInerpretion(const WordInterpretation &itp) { m_interpretations << itp; }

    Word &operator=(const Word &obj);
    void Clear();

    QString ToDisplayString(qint32 indent);
    QString ToRecordString(qint32 indent);

    void Record(const QString &pathfile = "");
    static bool IsEnglishWord(const QString &str);

    void Play(int volume);

private:
    void Parse(const QStringList &lines);
    void ParseWordInfo(const QString &text);

private:
    QString m_word;
    QString m_soundmark;
    QString m_hot;
    QString m_timestamp;
    QStringList m_tags;
    QList<WordInterpretation> m_interpretations;

    QString m_pathfile;
    QString m_type;

    QMediaPlayer *m_mediaplayer;
};

#endif // WORD_H
