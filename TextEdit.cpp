
#include "TextEdit.h"
#include <QFile>
#include <QRegExp>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>

TextEdit::TextEdit(const QString &pathfile,  QIODevice::OpenMode mode) :
    m_pathfile(pathfile),
    m_textbuf(new QStringList),
    m_changed(false),
    m_pc(0),
    m_mode(mode)
{
    QFile file(m_pathfile);

    if(!file.open(m_mode))
    {
        return;
    }

    QTextStream stream(&file);
    stream.setCodec("UTF-8");

    while(!stream.atEnd())
    {
        *m_textbuf << stream.readLine();
    }

    file.close();
}

TextEdit::TextEdit(const QStringList &lines) :
    m_pathfile(""),
    m_changed(false),
    m_pc(0)
{
    m_textbuf = new QStringList(lines);
}

TextEdit::~TextEdit()
{
    if(nullptr != m_textbuf )
    {
        if(m_changed && !m_pathfile.isEmpty())
        {
            QFile file(m_pathfile);
            if(file.open(m_mode))
            {
                QTextStream stream(&file);
                stream.setCodec("UTF-8");

                for(int i = 0; i < m_textbuf->count(); i++)
                {
                     stream << (*m_textbuf)[i] << endl;
                }

                stream.flush();
                file.flush();
                file.close();
            }
        }

        delete m_textbuf;
        m_textbuf = nullptr;
    }
}

bool TextEdit::Replace(const QRegularExpression &begin, const QRegularExpression &end, const QString &t)
{
    int ia = m_textbuf->indexOf(begin);
    int ib = m_textbuf->indexOf(end, ia + 1) ;

    if(-1 == ia)
    {
        return false;
    }

    if(-1 == ib)
    {
        ib = m_textbuf->count();
    }

    for(int i = 0; i < (ib - ia); i++)
    {
        m_textbuf->removeAt(ia);
    }

    m_textbuf->insert(ia, t);
    m_changed = true;
    return true;
}

QStringList TextEdit::Find(const QRegularExpression &begin, const QRegularExpression &end)
{
    QStringList found;

    if(m_pc > m_textbuf->count())
    {
        return found;
    }

    int pa = m_textbuf->indexOf(begin, m_pc);
    if(-1 == pa)
    {
        return found;
    }

    int pb = m_textbuf->indexOf(end, pa + 1) ;
    if(-1 == pb)
    {
        pb = m_textbuf->count();
    }

    for(int i = 0; i < (pb - pa); i++)
    {
        found << m_textbuf->at(pa + i);
    }

    m_pc = pb + 1;
    return found;
}

TextEdit &TextEdit::operator<<(const QString &str)
{
    *m_textbuf << str;
    m_changed = true;
    return *this;
}

void TextEdit::RemoveSpaceLines()
{
    for(int i = 0; i < m_textbuf->count(); i++)
    {
        if(m_textbuf->at(i).trimmed().remove("\n").remove("\r").remove("\t").trimmed().isEmpty())
        {
            m_textbuf->removeAt(i);
        }
    }
}
