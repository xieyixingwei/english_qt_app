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
            if(Word::IsWordStr(line))
            {
                afterLines << ExtractPatternTense(line);
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

QString Sentence::ExtractPatternTense(const QString &line)
{
    QRegularExpression rex(QString("(?<=\\<).*?(?=\\>)"));
    QRegularExpressionMatch match = rex.match(line);
    QString result = line;
    result.remove(rex);

    if(match.hasMatch())
    {
        QStringList matchStrs = match.captured(0).split(";");

        for(int i = 0; i < matchStrs.count(); i++)
        {
            rex.setPattern(QString("(?<=pattern:).*?(?=\\b)"));
            match = rex.match(matchStrs[i]);
            if(match.hasMatch())
            {
                m_pattern = match.captured(0).trimmed().split(",");
            }
            else
            {
                rex.setPattern(QString("(?<=tense:).*?(?=\\b)"));
                match = rex.match(matchStrs[i]);
                if(match.hasMatch())
                {
                    m_tense = match.captured(0).trimmed().split(",");
                }
            }
        }
    }

    return result;
}

void Sentence::Display(QTextEdit *testedit)
{
    testedit->append(ToDisplayString(m_index) + "\n");
}

void Sentence::Update()
{

}

QString Sentence::ToRecordString()
{
    QString record;
    QStringList tagList;
    QString tag;
    QString indent(m_indent, ' ');

    if(!m_pattern.isEmpty())
    {
        tagList << QString("pattern: %1").arg(m_pattern.join(", "));
    }
    if(!m_tense.isEmpty())
    {
        tagList << QString("tense: %1").arg(m_tense.join(", "));
    }
    if(!tagList.isEmpty())
    {
        tag = "<" + tagList.join("; ") + ">";
    }

    if(!m_sentence.first.isEmpty())
    {
        if(Word::IsWordStr(m_sentence.first))
        {
            record += QString(indent + "* %1 %2  \n").arg(m_sentence.first).arg(tag);
        }
        else
        {
            record += QString(indent + "* %1  \n").arg(m_sentence.first);
        }
    }

    if(!m_sentence.second.isEmpty())
    {
        if(Word::IsWordStr(m_sentence.second))
        {
            record += QString(indent + "* %1 %2  \n").arg(m_sentence.second).arg(tag);
        }
        else
        {
            record += QString(indent + "* %1  \n").arg(m_sentence.second);
        }
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

    if(!m_sentence.first.isEmpty())
    {
        display += QString(indent + "%1 %2\n").arg(indexList[0]).arg(m_sentence.first);
    }

    if(!m_sentence.first.isEmpty() && !m_sentence.second.isEmpty())
    {
        display += QString(indent + "%1 %2\n").arg(indexList[1]).arg(m_sentence.second);
    }
    else if(m_sentence.first.isEmpty() && !m_sentence.second.isEmpty())
    {
        display += QString(indent + "%1 %2\n").arg(indexList[0]).arg(m_sentence.second);
    }

    return display;
}

void Sentence::Record(const QString &pathfile)
{
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
    m_indent = 0;
    m_index = 0;
}

