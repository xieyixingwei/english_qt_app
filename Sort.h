#ifndef SORT_H
#define SORT_H

#include<QVector>
#include <QDebug>
#include <QtAlgorithms>

template <class T>
class Sort
{
public:
    Sort()
    {
        m_vector = new (QVector<T>);
    }

    ~Sort()
    {
        if(nullptr != m_vector)
        {
            delete m_vector;
            m_vector = nullptr;
        }
    }

    void Append(const T &t) { m_vector->push_back(t); }
    QVector<T>* &SortItems()
    {
        qSort(m_vector->begin(), m_vector->end());
        return m_vector;
    }

    inline QVector<T> *& Vector() const { return m_vector; }

    void Debug()
    {
        for(typename QVector<T>::iterator it = m_vector->begin(); it != m_vector->end(); it++)
        {
            qDebug() << (*it)->Text();
        }
    }

protected:
    QVector<T> *m_vector;
};

class SortTimeStamp
{
public:
    SortTimeStamp(const QString &text);
    SortTimeStamp();
    ~SortTimeStamp() = default;

    inline qint64 Value() const { return m_timestamp; }
    inline const QString &Text() const { return m_text; }
    friend bool operator<(const SortTimeStamp &a, const SortTimeStamp &b);

private:
    QString m_text;
    qint64 m_timestamp;
};

class SortHot
{
public:
    SortHot(const QString &text);
    SortHot();
    ~SortHot() = default;

    inline qint64 Value() const { return m_hot; }
    inline const QString &Text() const { return m_text; }
    friend bool operator<(const SortHot &a, const SortHot &b);

private:
    QString m_text;
    qint64 m_hot;
};


#endif // SORT_H
