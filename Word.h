#ifndef WORD_H
#define WORD_H

#include "Search.h"

#include <QString>
#include <QStringList>
#include <QList>
#include <QPair>
#include <QMediaPlayer>

class WordExample
{
public:
    WordExample(const QStringList &text) { Parse(text); }
    WordExample() = default;
    ~WordExample() = default;

    inline QStringList GetTag() { return m_tag; }
    inline void SetTag(const QStringList &tag) { m_tag = tag; }
    inline void AddTag(const QString &tag) { m_tag << tag; }

    inline QPair<QString, QString> GetExample() { return m_example; }
    inline void SetExample(const QPair<QString, QString> &example) { m_example = example; }

    QString ToRecordString();
    QString ToDisplayString(int index);

private:
    void Parse(const QStringList &text);
    QString ParseOneLineExample(const QString &line);

public:
    QStringList m_tag;
    QPair<QString, QString> m_example;
};

class WordInterpretation
{
public:
    WordInterpretation(const QStringList &text) { Parse(text); }
    WordInterpretation() = default;
    ~WordInterpretation() = default;

    inline void SetPos(const QString &pos) { m_pos = pos; }
    inline QString GetPos() const { return m_pos; }
    inline void AddMean(const QString &mean) { m_mean << mean; }
    inline void AddMean(const QStringList &means) { m_mean << means; }
    inline void SetMean(const QStringList &mean) { m_mean = mean; }
    inline QStringList GetMean() const { return m_mean; }

    inline void AddExample(const WordExample &exp) { m_examples << exp; }
    inline void AddExample(const QList<WordExample> &exps) { m_examples << exps; }
    inline QList<WordExample> GetExample() const { return m_examples; }

    QString ToRecordString();
    QString ToDisplayString();
    static QList<WordInterpretation> ToWordInterpretationList(const QStringList &text);

private:
    void Parse(const QStringList &text);

private:
    QString m_pos;
    QStringList m_mean;
    QList<WordExample> m_examples;
};

class Word : public SearchResult
{
public:
    Word(const QStringList &text) : m_mediaplayer(new QMediaPlayer) { Parse(text); }
    Word(const Word & obj);
    Word() { }
    ~Word() = default;

    inline virtual void SetType(const QString &type) { m_type = type; }
    inline virtual QString GetType() { return m_type; }
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
    inline void SetTags(const QStringList &tags) { m_tags = tags; }
    inline void AddTag(const QString &tag) { m_tags << tag; }
    inline QStringList GetSorts() const { return m_sorts; }
    inline void SetSorts(const QStringList &sorts) { m_sorts = sorts; }
    inline void AddSort(const QString &sort) { m_sorts << sort; }

    inline QList<WordInterpretation> GetInerpretion() { return m_interpretation; }
    inline void SetInerpretion(const QList<WordInterpretation> &itp) { m_interpretation = itp; }
    inline void AddInerpretion(const WordInterpretation &itp) { m_interpretation << itp; }
    void MergeInerpretion(const QList<WordInterpretation> &itps);
    void MergeInerpretion(const WordInterpretation &itps);

    Word &operator=(const Word & obj);
    void Clear();

    QString ToDisplayString();
    QString ToRecordString();
    QString ToInerpretionDisplayString ();

    void Record(const QString &pathfile = "");
    static bool IsWordStr(const QString &str);

    void Play(int volume);

private:
    void Parse(const QStringList &text);
    void ParseWordInfor(QString text);

private:
    QString m_word;
    QString m_soundmark;
    QString m_hot;
    QString m_timestamp;
    QStringList m_tags;
    QStringList m_sorts;
    QList<WordInterpretation> m_interpretation;

    QString m_pathfile;
    QString m_type;

    QMediaPlayer *m_mediaplayer;
};

#endif // WORD_H
