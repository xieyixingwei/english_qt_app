#include "Sentence.h"
#include "TextEdit.h"
#include "Setting.h"
#include "DialogSet.h"

Sentence::Sentence(const QStringList &str)
{
    QString a = str[0].trimmed().remove(QRegExp("^\\*"));
    QString b = str.count() > 1 ? str[1].trimmed().remove(QRegExp("^\\*")) : "";
    QString c = str.count() > 2 ? str[2].trimmed().remove(QRegExp("^\\*")) : "";

    if(a.isEmpty())
    {
        m_sentence = QPair<QString, QString>(b,c);
    }

    if(c.isEmpty())
    {
        m_sentence = QPair<QString, QString>(a,b);
    }
}

void Sentence::Display(QPlainTextEdit *testedit)
{
    testedit->appendPlainText(ToDisplayString() + "\n");
}

void Sentence::Update()
{

}

QString Sentence::ToRecordString()
{
    return QString("%1\n%2\n").arg(m_sentence.first).arg(m_sentence.second);
}

QString Sentence::ToDisplayString()
{
    return QString("%1\n%2\n").arg(m_sentence.first).arg(m_sentence.second);
}

void Sentence::Record(const QString &pathfile)
{
    TextEdit file(pathfile.isEmpty() ? m_pathfile : pathfile);

    file << ToRecordString();
}
