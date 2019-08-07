#include "Word.h"
#include "TextEdit.h"
#include "Setting.h"
#include "DialogSet.h"

#include <QRegExp>
#include <QRegularExpression>
#include <QDebug>
#include <QDateTime>

void WordInterpretation::Parse(const QStringList &lines)
{
    TextEdit text = TextEdit(lines);
    text.RemoveSpaceLines();

    if(text.Buf().count() == 0)
    {
        return;
    }

    QRegularExpression rex(QString("(?P<pos>.*?(?=\\.))"));
    QRegularExpressionMatch matched = rex.match(text.Buf().at(0));
    if(matched.hasMatch())
    {
        m_pos = matched.captured("pos").remove("+").trimmed();
    }

    rex.setPattern(QString("(?P<means>(?<=\\.).*)"));
    matched = rex.match(text.Buf().at(0));
    if(matched.hasMatch())
    {
        QString means = matched.captured("means").trimmed();
        if(!means.isEmpty())
        {
            m_means = means.split(";");
        }
    }

    QString a;
    QString b;
    int i = 1;
    while(i < text.Buf().count())
    {
        a.clear();
        b.clear();

        a = text.Buf()[i++].remove("*");

        if(i < text.Buf().count() &&
                Sentence::IsEnglishSentence(a) &&
                !Sentence::IsEnglishSentence(text.Buf()[i].remove("*")))
        {
            b = text.Buf()[i++];
        }

        m_examples << Sentence(a, b);
    }
}

QString WordInterpretation::ToRecordString(qint32 indent)
{
    QString indentStr(indent, ' ');
    QString record = QString(indentStr + "+ %1. %2\n").arg(m_pos).arg(m_means.join(";"));

    for(int i = 0; i < m_examples.count(); i++)
    {
        m_examples[i].SetIndent((indent == 0) ? 4 : indent * 2);
        record += m_examples[i].ToRecordString();
    }

    return record;
}

QString WordInterpretation::ToDisplayString(qint32 indent)
{
    QString indentStr(indent, ' ');
    QString display = QString(indentStr + "%1. %2\n").arg(m_pos).arg(m_means.join(";"));

    for(int i = 0; i < m_examples.count(); i++)
    {
        m_examples[i].SetIndent((indent == 0) ? 4 : indent * 2);
        display += m_examples[i].ToDisplayString(i + 1);
    }

    return display;
}

void WordInterpretation::Clear()
{
    m_pos.clear();
    m_means.clear();
    m_examples.clear();
}

QList<WordInterpretation> WordInterpretation::WordInterpretationList(const QStringList &lines)
{
    QList<WordInterpretation> interps;
    TextEdit text = TextEdit(lines);
    text.RemoveSpaceLines();

    QStringList strs;
    for(int i = 0; i < text.Buf().count(); i++)
    {
        if(!strs.isEmpty() && !text.Buf().at(i).startsWith("  "))
        {
            interps << WordInterpretation(strs);
            strs.clear();
        }
        else if(!strs.isEmpty() && (i + 1) == text.Buf().count())
        {
            strs << text.Buf()[i].remove(QRegularExpression("\\(\\d+\\)"));
            interps << WordInterpretation(strs);
            strs.clear();
        }

        strs << text.Buf()[i].remove(QRegularExpression("\\(\\d+\\)"));
    }

    return interps;
}

// Word -------------------------------------------------------------------------

Word::Word(const Word & obj) : m_mediaplayer(new QMediaPlayer)
{
    m_word = obj.m_word;
    m_soundmark = obj.m_soundmark;
    m_hot = obj.m_hot;
    m_timestamp = obj.m_timestamp;
    m_tags = obj.m_tags;
    m_interpretations = obj.m_interpretations;
    m_pathfile = obj.m_pathfile;
    m_type = obj.m_type;
}

Word &Word::operator=(const Word & obj)
{
    m_word = obj.m_word;
    m_soundmark = obj.m_soundmark;
    m_hot = obj.m_hot;
    m_timestamp = obj.m_timestamp;
    m_tags = obj.m_tags;
    m_interpretations = obj.m_interpretations;
    m_pathfile = obj.m_pathfile;
    m_type = obj.m_type;
    return *this;
}

void Word::Clear()
{
    m_word.clear();
    m_soundmark.clear();
    m_hot.clear();
    m_timestamp.clear();
    m_tags.clear();
    m_interpretations.clear();
    m_pathfile.clear();
    m_type.clear();
}

void Word::Parse(const QStringList &lines)
{
    TextEdit text = TextEdit(lines);
    text.RemoveSpaceLines();

    ParseWordInfo(text.Buf().takeFirst());
    m_interpretations = WordInterpretation::WordInterpretationList(text.Buf());
}

void Word::ParseWordInfo(const QString &text)
{
    QRegularExpression rex(QString("(?P<word>(?<=\\-)[ ]*\\w+)"));
    QRegularExpressionMatch matched = rex.match(text);
    if(matched.hasMatch())
    {
        m_word = matched.captured("word").trimmed().toLower();
    }

    rex.setPattern("(?P<soundmark>(?<=\\[).*?(?=\\]))");
    matched = rex.match(text);
    if(matched.hasMatch())
    {
        m_soundmark = matched.captured("soundmark");
    }

    rex.setPattern("(?P<hot>(?<=hot:)\\d+)");
    matched = rex.match(text);
    if(matched.hasMatch())
    {
        m_hot = matched.captured("hot");
    }

    rex.setPattern("(?P<timestamp>(?<=timestamp:)\\d+)");
    matched = rex.match(text);
    if(matched.hasMatch())
    {
        m_timestamp = matched.captured("timestamp");
    }

    rex.setPattern("(?P<tag>(?<=tag:).*?(?=>))");
    matched = rex.match(text);
    if(matched.hasMatch())
    {
        m_tags = matched.captured("tag").split(",");
    }
}

QString Word::ToRecordString(qint32 indent)
{
    QString wordInfo;
    wordInfo += QString("hot:%1").arg(m_hot.isEmpty() ? "1" : m_hot);
    wordInfo += QString(" timestamp:%1").arg(m_timestamp.isEmpty() ? QDateTime::currentDateTime().toString("yyMMddhhmm") : m_timestamp);
    if(!m_tags.isEmpty())
    {
        wordInfo += QString(" tag:%1").arg(m_tags.join(","));
    }

    QString record = QString("- %1").arg(m_word);
    if(!m_soundmark.isEmpty())
    {
        record += QString(" [%1]").arg(m_soundmark);
    }

    if(!wordInfo.isEmpty())
    {
        record += QString(" <%1>  \n").arg(wordInfo);
    }

    for(QList<WordInterpretation>::Iterator it = m_interpretations.begin(); it != m_interpretations.end(); it++)
    {
        record += it->ToRecordString(indent);
    }

    return record;
}

QString Word::ToDisplayString(qint32 indent)
{
    QString display = QString("%1").arg(m_word);
    if(!m_soundmark.isEmpty())
    {
        display += QString(" [%1]").arg(m_soundmark);
    }

    display += "\n";

    for(int i = 0; i < m_interpretations.count(); i++)
    {
        display += m_interpretations[i].ToDisplayString(indent);
    }

    return display;
}

void Word::Record(const QString &pathfile)
{
    TextEdit file(pathfile.isEmpty() ? m_pathfile : pathfile);

    bool res = file.Replace(QRegularExpression("^[ ]*-[ ]*" + m_word + ".*")
                        , QRegularExpression("^[ ]*-[ ]*.*")
                        , ToRecordString(4));
    if(false == res)
    {
        file << ToRecordString(4);
    }
}

bool Word::IsEnglishWord(const QString &str)
{
    QRegularExpression rex(QString("(?P<word>[a-zA-Z\\- ]*)"));
    QRegularExpressionMatch match = rex.match(str);
    if(match.hasMatch() && str == match.captured("word"))
    {
        return true;
    }

    return false;
}

void Word::Display(QTextEdit *testedit)
{
    testedit->append(ToDisplayString(4));
}

void Word::Update()
{
    if(SETS[KEY_UPDATE_HOT].toBool())
    {
        m_hot = QString::number(m_hot.toULong() + 1);
    }

    if(SETS[KEY_UPDATE_TIMESTAMP].toBool())
    {
        m_timestamp = QDateTime::currentDateTime().toString("yyMMddhhmm");
    }

    Record();
}

void Word::Play(int volume)
{
    if(m_word.isEmpty())
    {
        return;
    }

    //m_mediaplayer->setMedia(QUrl::fromLocalFile(SETS[DialogSet::KEY_SOUND_DIR].toString() + m_word + "_1.mp3"));
    m_mediaplayer->setVolume(volume);
    m_mediaplayer->play();
}
