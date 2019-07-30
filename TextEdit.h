#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

class TextEdit
{
public:
    typedef enum {
        E_HOLD_NO = 0,
        E_HOLD_A,
        E_HOLD_B,
        E_HOLD_AB
    } E_MODE;

public:
    TextEdit(const QString &pathfile);
    TextEdit(const QStringList &strlist);
    ~TextEdit();

    inline QStringList &Buf() { return *m_textbuf; }
    inline void Clear() { m_textbuf->clear(); }

    inline bool Exist(const QRegularExpression &re) { return (-1 != m_textbuf->indexOf(re)); }
    inline QStringList Filter(const QRegularExpression &re) { return m_textbuf->filter(re); }
    inline void Replace(const QRegularExpression &a, const QString &b) { m_textbuf->replaceInStrings(a, b); m_change = true; }

    QStringList FindBetween(const QRegularExpression &a, const QRegularExpression &b, E_MODE mode);
    QList<QStringList> FindAllBetween(const QRegularExpression &a, const QRegularExpression &b, E_MODE mode = E_HOLD_A);
    void InsertBehind(const QRegularExpression &a, const QString &b);
    void InsertBefore(const QRegularExpression &a, const QString &b);
    bool ReplaceBetween(const QRegularExpression &a, const QRegularExpression &b, const QString &c, E_MODE mode = E_HOLD_B);

    TextEdit &operator<<(const QString &str);
    void RemoveSpaceLines();

private:
    QString m_pathfile;
    QStringList *m_textbuf;
    bool m_change;
};

#endif // TextEdit_H
