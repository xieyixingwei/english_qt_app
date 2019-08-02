#include "Sort.h"
#include <QRegularExpression>
#include <QDateTime>

SortTimeStamp::SortTimeStamp(const QString &text) :
    m_text(text)
{
    QRegularExpression rex(QString("(?P<timestamp>(?<=timestamp:)\\d+)"));
    QRegularExpressionMatch matched = rex.match(text);
    if(matched.hasMatch())
    {
        m_timestamp = QDateTime::fromString(matched.captured("timestamp").trimmed(), "yyMMddhhmm").toSecsSinceEpoch();
    }
    else
    {
        m_timestamp = 0;
    }
}

SortTimeStamp::SortTimeStamp() :
    m_text(""),
    m_timestamp(0)
{

}

bool operator<(const SortTimeStamp &a, const SortTimeStamp &b)
{
    return a.Value() > b.Value();
}



SortHot::SortHot(const QString &text) :
    m_text(text)
{
    QRegularExpression rex(QString("(?P<hot>(?<=hot:)\\d+)"));
    QRegularExpressionMatch matched = rex.match(text);
    if(matched.hasMatch())
    {
        m_hot = matched.captured("hot").trimmed().toLong();
    }
    else
    {
        m_hot = 0;
    }
}

SortHot::SortHot() :
    m_text(""),
    m_hot(0)
{

}

bool operator<(const SortHot &a, const SortHot &b)
{
    return a.Value() > b.Value();
}
