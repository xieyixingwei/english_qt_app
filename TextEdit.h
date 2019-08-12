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
    TextEdit(const QString &pathfile,  QIODevice::OpenMode mode = QIODevice::ReadWrite | QIODevice::Text);
    TextEdit(const QStringList &lines);
    ~TextEdit();

    inline QStringList &Buf() { return *m_textbuf; }
    bool Replace(const QRegularExpression &begin, const QRegularExpression &end, const QString &t);
    QStringList Find(const QRegularExpression &begin, const QRegularExpression &end);
    QStringList FindSentences();
    QStringList FindSentencesOfWord();

    TextEdit &operator<<(const QString &str);
    void RemoveSpaceLines();

private:
    QString m_pathfile;
    QStringList *m_textbuf;
    bool m_changed;
    int m_pc;
    QIODevice::OpenMode m_mode;
};

#endif // TextEdit_H
