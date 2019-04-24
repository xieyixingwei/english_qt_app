
#include "TextEdit.h"
#include <QFile>
#include <QRegExp>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>

TextEdit::TextEdit(const QString &pathfile) :
    m_pathfile(pathfile), m_textbuf(new QStringList), m_change(false)
{
    m_pathfile = pathfile;
    QFile file(m_pathfile);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
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

TextEdit::TextEdit(const QStringList &strlist) :
    m_pathfile(""), m_change(false)
{
    m_textbuf = new QStringList;
    *m_textbuf << strlist;
}

TextEdit::~TextEdit()
{
    if(nullptr != m_textbuf)
    {
        if(m_change && !m_pathfile.isEmpty())
        {
            QFile file(m_pathfile);
            if(file.open(QIODevice::WriteOnly | QIODevice::Text))
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

QStringList TextEdit::FindBetween(const QRegularExpression &a, const QRegularExpression &b, E_MODE mode)
{
    QStringList found;
    int ia = m_textbuf->indexOf(a);
    int ib = m_textbuf->indexOf(b, ia + 1);

    if(-1 == ia)
    {
        return found;
    }

    if(-1 == ib)
    {
        ib = m_textbuf->count();
    }

    if(E_HOLD_A == mode || E_HOLD_AB == mode)
    {
        found << (*m_textbuf)[ia];
    }

    for(int i = ia + 1; i < ib; i++)
    {
        found << (*m_textbuf)[i];
    }

    if((m_textbuf->count() != ib) && (E_HOLD_B == mode || E_HOLD_AB == mode))
    {
        found << (*m_textbuf)[ib];
    }

    return found;
}

QList<QStringList> TextEdit::FindAllBetween(const QRegularExpression &a, const QRegularExpression &b, TextEdit::E_MODE mode)
{
    QList<QStringList> result;

    int p = 0;
    while(p < m_textbuf->count())
    {
        QStringList found;
        int ia = m_textbuf->indexOf(a, p);
        int ib = m_textbuf->indexOf(b, ia + 1);

        if(-1 == ia)
        {
            return result;
        }

        if(-1 == ib)
        {
            ib = m_textbuf->count();
        }

        if(E_HOLD_A == mode || E_HOLD_AB == mode)
        {
            found << (*m_textbuf)[ia];
        }

        for(int i = ia + 1; i < ib; i++)
        {
            found << (*m_textbuf)[i];
        }

        if((m_textbuf->count() != ib) && (E_HOLD_B == mode || E_HOLD_AB == mode))
        {
            found << (*m_textbuf)[ib];
        }

        result << found;
        p = ib;
    }

    return result;
}

void TextEdit::InsertBehind(const QRegularExpression &a, const QString &b)
{
    int i = m_textbuf->indexOf(a);

    if(-1 != i)
    {
        m_textbuf->insert(i + 1, b);
        m_change = true;
    }
}

void TextEdit::InsertBefore(const QRegularExpression &a, const QString &b)
{
    int i = m_textbuf->indexOf(a);

    if(-1 != i)
    {
        m_textbuf->insert(i, b);
        m_change = true;
    }
}

bool TextEdit::ReplaceBetween(const QRegularExpression &a, const QRegularExpression &b, const QString &c, E_MODE mode)
{
    int ia = m_textbuf->indexOf(a);
    int ib = m_textbuf->indexOf(b, ia + 1) ;

    if(-1 == ia)
    {
        return false;
    }

    if(-1 == ib)
    {
        ib = m_textbuf->count();
    }

    for(int i = 1; i < (ib - ia); i++)
    {
        m_textbuf->removeAt(ia + 1);
    }

    m_textbuf->insert(ia + 1, c);

    if(E_HOLD_NO == mode || E_HOLD_B == mode)
    {
        m_textbuf->removeAt(ia);
    }

    if((m_textbuf->count() != ib) && (E_HOLD_NO == mode || E_HOLD_A == mode))
    {
        m_textbuf->removeAt(ia + 2);
    }

    m_change = true;
    return true;
}

TextEdit &TextEdit::operator<<(const QString &str)
{
    *m_textbuf << str;
    m_change = true;
    return *this;
}
