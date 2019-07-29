#include "Word.h"
#include "TextEdit.h"
#include "Setting.h"
#include "DialogSet.h"

#include <QRegExp>
#include <QRegularExpression>
#include <QDebug>
#include <QDateTime>


void WordInterpretation::Parse(const QStringList &text)
{
    QStringList tmp = text[0].split(".");

    m_pos = tmp[0].trimmed().remove(QRegExp("^\\+")).trimmed();

    if(tmp.count() > 1)
    {
        m_mean = tmp[1].trimmed().remove(" ").split(",");
    }

    int i = 1;
    while(i < text.count())
    {
        QStringList expstr;
        expstr.clear();

        expstr << text[i++];
        if(i < text.count())
        {
            expstr << text[i++];
        }

        m_examples << Sentence(expstr);
    }
}

QString WordInterpretation::ToRecordString()
{
    QString record = QString("    + %1. %2\n").arg(m_pos).arg(m_mean.join(","));

    for(int i = 0; i < m_examples.count(); i++)
    {
        record += m_examples[i].ToRecordString();
    }

    return record;
}

QString WordInterpretation::ToDisplayString()
{
    QString display = QString("    %1. %2\n").arg(m_pos).arg(m_mean.join(","));

    for(int i = 0; i < m_examples.count(); i++)
    {
        display += m_examples[i].ToDisplayString(i + 1);
    }

    return display;
}

QList<WordInterpretation> WordInterpretation::ToWordInterpretationList(const QStringList &text)
{
    QList<WordInterpretation> itps;

    TextEdit edit(text);

    QList<QStringList> itpstring = edit.FindAllBetween(QRegularExpression("^[ ]*\\+.*"), QRegularExpression("^[ ]*\\+.*"));

    for(int i = 0; i < itpstring.count(); i++)
    {
        WordInterpretation witp(itpstring[i]);
        itps << witp;
    }

    return itps;
}

// Word -------------------------------------------------------------------------

Word::Word(const Word & obj) : m_mediaplayer(new QMediaPlayer)
{
    m_word = obj.m_word;
    m_soundmark = obj.m_soundmark;
    m_hot = obj.m_hot;
    m_timestamp = obj.m_timestamp;
    m_interpretation = obj.m_interpretation;
    m_pathfile = obj.m_pathfile;
    m_type = obj.m_type;
}

Word &Word::operator=(const Word & obj)
{
    m_word = obj.m_word;
    m_soundmark = obj.m_soundmark;
    m_hot = obj.m_hot;
    m_timestamp = obj.m_timestamp;
    m_interpretation = obj.m_interpretation;
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
    m_interpretation.clear();
    m_pathfile.clear();
    m_type.clear();
}

void Word::Parse(const QStringList &text)
{
    ParseWordInfor(text[0]);
    m_interpretation = WordInterpretation::ToWordInterpretationList(text);
}

void Word::ParseWordInfor(QString text)
{
    QRegExp rex(QString("[a-z\\-A-Z]+"));

    if(-1 != rex.indexIn(text.trimmed().remove(QRegExp("^-")), 0))
    {
        m_word = rex.cap(0);
    }

    rex.setPattern("\\[.*\\]");
    if(-1 != rex.indexIn(text, 0))
    {
        m_soundmark = rex.cap(0).remove("[").remove("]");
    }

    rex.setPattern("hot:\\d*");
    if(-1 != rex.indexIn(text, 0))
    {
        QString hot = rex.cap(0);

        rex.setPattern("\\d+");
        if(-1 != rex.indexIn(hot, 0))
        {
            m_hot = rex.cap(0);
        }
    }

    rex.setPattern("timestamp:\\d*");
    if(-1 != rex.indexIn(text, 0))
    {
        QString timestamp = rex.cap(0);

        rex.setPattern("\\d+");
        if(-1 != rex.indexIn(timestamp, 0))
        {
            m_timestamp = rex.cap(0);
        }
    }
}

QString Word::ToRecordString()
{
    QString record;

    record += QString("- %1").arg(m_word.toLower());

    if(!m_soundmark.isEmpty())
    {
        record += QString(" [%1]").arg(m_soundmark);
    }

    record += QString(" <hot:%1").arg(m_hot.isEmpty() ? "1" : m_hot);

    record += QString(" timestamp:%1").arg(m_timestamp.isEmpty() ? QDateTime::currentDateTime().toString("yyMMddhhmm") : m_timestamp);

    record += ">\n";

    for(QList<WordInterpretation>::Iterator it = m_interpretation.begin(); it != m_interpretation.end(); it++)
    {
        record += it->ToRecordString();
    }

    return record;
}

QString Word::ToDisplayString()
{
    QString display = QString("%1").arg(m_word);

    if(!m_soundmark.isEmpty())
    {
        display += QString(" [%1]").arg(m_soundmark);
    }

    display += "\n";

    return display + QString("%1").arg(ToInerpretionDisplayString());
}

QString Word::ToInerpretionDisplayString()
{
    QString display;

    for(int i = 0; i < m_interpretation.count(); i++)
    {
        display += m_interpretation[i].ToDisplayString();
    }

    return display;
}

void Word::MergeInerpretion(const QList<WordInterpretation> &itps)
{
    for(int i = 0; i < itps.count(); i++)
    {
        MergeInerpretion(itps[i]);
    }
}

void Word::MergeInerpretion(const WordInterpretation &itps)
{
    for(int i = 0; i < m_interpretation.count(); i++)
    {
        if(itps.GetPos() == m_interpretation[i].GetPos())
        {
            m_interpretation[i].AddMean(itps.GetMean());
            m_interpretation[i].AddExample(itps.GetExample());
            return;
        }
    }

    m_interpretation << itps;
}

void Word::Record(const QString &pathfile)
{
    TextEdit file(pathfile.isEmpty() ? m_pathfile : pathfile);

    bool res = file.ReplaceBetween(QRegularExpression("^[ ]*-[ ]*" + m_word + ".*")
                        , QRegularExpression("^[ ]*-[ ]*.*")
                        , ToRecordString());
    if(false == res)
    {
        file << ToRecordString();
    }
}

bool Word::IsWordStr(const QString &str)
{
    for(int i = 0; i < str.count(); i++)
    {
        if(!(('a' <= str[i] && 'z' >= str[i]) || ('A' <= str[i] && 'Z' >= str[i])))
        {
            return false;
        }
    }

    return true;
}

void Word::Display(QTextEdit *testedit)
{
    testedit->append(ToDisplayString());
}

void Word::Update()
{
    if(SETS[DialogSet::KEY_UPDATE_HOT].toBool())
    {
        m_hot = QString::number(m_hot.toULong() + 1);
    }

    if(SETS[DialogSet::KEY_UPDATE_TIMESTAMP].toBool())
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

    m_mediaplayer->setMedia(QUrl::fromLocalFile(SETS[DialogSet::KEY_SOUND_DIR].toString() + m_word + "_1.mp3"));
    m_mediaplayer->setVolume(volume);
    m_mediaplayer->play();
}
