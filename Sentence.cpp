#include "Sentence.h"
#include "TextEdit.h"
#include "Setting.h"
#include "DialogSet.h"
#include "Word.h"

#include <QRegularExpression>

Sentence::Sentence(const QStringList &str) :
    m_indent(0),
    m_index(0)
{
    QStringList src = str;
    QStringList text;
    for(int i = 0; i < src.count(); i++)
    {
        QString tmp = src[i].remove("*").trimmed();
        if(!tmp.isEmpty())
        {
            text << tmp;
        }
    }

    QString a;
    QString b;

    if(Word::IsWordStr(text[0]))
    {
        a = ExtractPatternTense(text[0]);
        b = text[1];
    }
    else if(Word::IsWordStr(text[1]))
    {
        a = ExtractPatternTense(text[1]);
        b = (text[0]);
    }

    m_sentence = QPair<QString, QString>(a.remove("\n").remove("\r").remove("\t").trimmed(),
                                         b.remove("\n").remove("\r").remove("\t").trimmed());
}

QString Sentence::ExtractPatternTense(const QString &line)
{
    QRegularExpression rex(QString("(?<=\\<).*?(?=\\>)"));
    QRegularExpressionMatch match = rex.match(line);
    QString result = line;

    if(match.hasMatch())
    {
        QStringList matchStrs = match.captured(0).split(",");

        for(int i = 0; i < matchStrs.count(); i++)
        {
            rex.setPattern(QString("(?<=pattern:).*?(?=\\b)"));
            match = rex.match(matchStrs[i]);
            if(match.hasMatch())
            {
                m_pattern = match.captured(0).trimmed();
            }
            else
            {
                rex.setPattern(QString("(?<=tense:).*?(?=\\b)"));
                match = rex.match(matchStrs[i]);
                if(match.hasMatch())
                {
                    m_tense = match.captured(0).trimmed();
                }
            }
        }
        return result.remove(rex);
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
    QString record("");

    if(m_sentence.first.isEmpty())
    {
        return record;
    }

    QString indent(m_indent, ' ');

    record = QString(indent + "* %1\n").arg(m_sentence.first);

    if(!m_pattern.isEmpty())
    {
        record += QString(" <pattern: %1\n").arg(m_pattern);
    }

    if(!m_tense.isEmpty())
    {
        record += QString(", tense: %1").arg(m_tense);
    }

    record += QString(">  \n");

    if(!m_sentence.second.isEmpty())
    {
        record += QString(indent + "* %1  \n").arg(m_sentence.second);
    }

    return record;
}

QString Sentence::ToDisplayString(qint32 index = 0)
{
    QString display;

    if(m_sentence.first.isEmpty())
    {
        return display;
    }

    QString indent(m_indent, ' ');

    if(0 == index)
    {
        display += QString(indent + "%1\n").arg(m_sentence.first);
        if(!m_sentence.second.isEmpty())
        {
            display += QString(indent + "%1\n").arg(m_sentence.second);
        }
    }
    else
    {
        display += QString(indent + "(%1) %2\n").arg(index).arg(m_sentence.first);
        if(!m_sentence.second.isEmpty())
        {
            display += QString(indent + "    %1\n").arg(m_sentence.second);
        }
    }

    return display;
}

void Sentence::Record(const QString &pathfile)
{
    TextEdit file(pathfile.isEmpty() ? m_pathfile : pathfile);

    file << ToRecordString();
}
