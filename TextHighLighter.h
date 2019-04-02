#ifndef TEXTHIGHLIGHTER_H
#define TEXTHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QPair>

using oneline_rule_t = QPair<QRegularExpression, QTextCharFormat>;
using openclose_rule_t = struct openclose_rule {
        openclose_rule(const QRegularExpression &a, const QRegularExpression &b, const QTextCharFormat &fmt) :
        open_pattern(a), close_pattern(b), format(fmt)
        { }
        openclose_rule() = default;
        QRegularExpression open_pattern;
        QRegularExpression close_pattern;
        QTextCharFormat format;
    };

class TextHighLighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    TextHighLighter(QTextDocument *parent);
    ~TextHighLighter();

    inline void AddRule(const QRegularExpression &pattern, const QTextCharFormat &format) { m_olrules << oneline_rule_t(pattern, format); }
    inline void AddRule(const oneline_rule_t &rule) { m_olrules << rule; }
    inline void AddRule(const QVector<oneline_rule_t> &rules) { m_olrules << rules; }
    inline void Clear() { m_olrules.clear(); }
    inline void AddRule(const QRegularExpression &open_pattern, const QRegularExpression &close_pattern, const QTextCharFormat &format) { m_ocrules = openclose_rule_t(open_pattern, close_pattern, format); }

protected:
    virtual void highlightBlock(const QString &text);

private:
    QVector<oneline_rule_t> m_olrules;
    openclose_rule_t m_ocrules; // Support only one mult-line high lighter !!!
};

#endif // TEXTHIGHLIGHTER_H
