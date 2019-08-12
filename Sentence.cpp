#include "Sentence.h"
#include "TextEdit.h"
#include "Setting.h"
#include "DialogSet.h"
#include "Word.h"

#include <QRegularExpression>

Sentence::Sentence(const QStringList &lines) :
    m_indent(0),
    m_index(0)
{
    Parse(lines);
}

Sentence::Sentence(const QString &a, const QString &b) :
    m_indent(0),
    m_index(0)
{
    QStringList lines;
    lines << a << b;
    Parse(lines);
}

Sentence::Sentence() :
    m_indent(0),
    m_index(0)
{

}

void Sentence::SetSentence(const QString &a, const QString &b)
{
    QStringList lines;
    lines << a << b;
    Parse(lines);
}

void Sentence::Parse(const QStringList &lines)
{
    TextEdit text = TextEdit(lines);
    text.RemoveSpaceLines();

    if(text.Buf().count() == 0)
    {
        return;
    }

    QStringList afterLines;
    for(int i = 0; i < text.Buf().count(); i++)
    {
        QString line = text.Buf()[i].remove("*").remove("\n").remove("\r").remove("\t").trimmed();
        if(!line.isEmpty())
        {
            if(Sentence::IsEnglishSentence(line))
            {
                afterLines << ExtractTags(line);
            }
            else
            {
                afterLines << line;
            }
        }
    }

    afterLines << "" << "";  // prevent afterLines.count() < 2
    m_sentence = QPair<QString, QString>(afterLines[0], afterLines[1]);
}

QString Sentence::ExtractTags(const QString &line)
{
    QRegularExpression rex(QString("(?P<pat>(?<=pattern:).*?(?=[;>]))"));

    QRegularExpressionMatch match = rex.match(line);
    if(match.hasMatch())
    {
        m_pattern <<  match.captured("pat").trimmed().split(",");
    }

    rex.setPattern(QString("(?P<ten>(?<=tense:).*?(?=[;>]))"));
    match = rex.match(line);
    if(match.hasMatch())
    {
        m_tense <<  match.captured("ten").trimmed().split(",");
    }

    rex.setPattern(QString("(?P<timestamp>(?<=timestamp:).*?(?=[;>]))"));
    match = rex.match(line);
    if(match.hasMatch())
    {
        m_timestamp = match.captured("timestamp").trimmed();
    }

    return line.split("<")[0].trimmed();
}

void Sentence::Display(QTextEdit *testedit)
{
    testedit->append(ToDisplayString(m_index) + "\n");
}

void Sentence::Update()
{

}

QString Sentence::GetTags()
{
    QStringList tagList;

    if(!m_pattern.join(" ").trimmed().isEmpty())
    {
        tagList << QString("pattern:%1").arg(m_pattern.join(", "));
    }
    if(!m_tense.join(" ").trimmed().isEmpty())
    {
        tagList << QString("tense:%1").arg(m_tense.join(", "));
    }
    if(!m_timestamp.isEmpty())
    {
        tagList << QString("timestamp:%1").arg(m_timestamp);
    }
    if(!tagList.isEmpty())
    {
        return ("<" + tagList.join("; ") + ">");
    }

    return "";
}

QString Sentence::AttachTags(const QString &line, const QString &tags, const QString &suffix)
{
    QString tmp = line;
    if(Sentence::IsEnglishSentence(tmp.remove("*").remove(QRegularExpression("\\(\\d+\\)"))) && !tags.isEmpty())
    {
        return line + " " + tags + suffix;
    }

    return line + suffix;
}

QString Sentence::ToRecordString()
{
    QString record;
    QString indent(m_indent, ' ');

    if(!m_sentence.first.isEmpty())
    {
        record += AttachTags(QString(indent + "* %1").arg(m_sentence.first), GetTags(), "  \n");
    }

    if(!m_sentence.second.isEmpty())
    {
        record += AttachTags(QString(indent + "* %1").arg(m_sentence.second), GetTags(), "  \n");
    }

    return record;
}

QString Sentence::ToDisplayString(qint32 index = 0)
{
    QString display;
    QStringList indexList;
    QString indent(m_indent, ' ');

    if(index > 0)
    {
        indexList << QString("(%1)").arg(index);
        indexList << "   ";
    }
    else
    {
        indexList << " ";
        indexList << " ";
    }

    if(!m_sentence.first.isEmpty())
    {
        display += AttachTags(QString(indent + "%1 %2").arg(indexList[0]).arg(m_sentence.first), GetTags(), "\n");
    }

    if(!m_sentence.first.isEmpty() && !m_sentence.second.isEmpty())
    {
        display += AttachTags(QString(indent + "%1 %2").arg(indexList[1]).arg(m_sentence.second), GetTags(), "\n");
    }
    else if(m_sentence.first.isEmpty() && !m_sentence.second.isEmpty())
    {
        display += AttachTags(QString(indent + "%1 %2").arg(indexList[0]).arg(m_sentence.second), GetTags(), "\n");
    }

    return display;
}

void Sentence::Record(const QString &pathfile)
{
    if(m_timestamp.isEmpty())
    {
        m_timestamp = QDateTime::currentDateTime().toString("yyMMddhhmm");
    }

    TextEdit file(pathfile.isEmpty() ? m_pathfile : pathfile);
    file << ToRecordString();
}

void Sentence::Clear()
{
    m_sentence.first.clear();
    m_sentence.second.clear();
    m_pathfile.clear();
    m_pattern.clear();
    m_tense.clear();
    m_timestamp.clear();
    m_indent = 0;
    m_index = 0;
}

bool Sentence::IsEnglishSentence(const QString &str)
{
    QRegularExpression rex(QString("(?P<sentence>[a-zA-Z\\-\\.\\?\\!, ]*)"));
    QString line = str.split("<")[0];
    QRegularExpressionMatch matched = rex.match(line);
    if(matched.hasMatch() && line == matched.captured("sentence"))
    {
        return true;
    }

    return false;
}
