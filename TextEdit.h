#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QString>
#include <QFile>
#include <QTextStream>

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

    bool Exist(const QString &a);
    QStringList Find(const QString &a);
    QStringList FindBetween(const QString &a, const QString &b, E_MODE mode);
    QList<QStringList> FindAllBetween(const QString &a, const QString &b, E_MODE mode = E_HOLD_A);
    void InsertBehind(const QString &a, const QString &b);
    void InsertBefore(const QString &a, const QString &b);
    void Replace(const QString &a, const QString &b);
    bool ReplaceBetween(const QString &a, const QString &b, const QString &c, E_MODE mode = E_HOLD_B);

    TextEdit &operator<<(const QString &str);

private:
    QString m_pathfile;
    QStringList *m_textbuf;
    bool m_change;
};

#endif // TextEdit_H
