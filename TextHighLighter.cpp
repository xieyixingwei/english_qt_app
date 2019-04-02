#include "TextHighLighter.h"
#include <QDebug>

TextHighLighter::TextHighLighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    //QTextCharFormat keywordfmt;
    //keywordfmt.setFontWeight(QFont::Bold);
    //keywordfmt.setForeground(Qt::darkBlue);

    //m_rules << rule_t(QRegularExpression("\\bchar\\b"), keywordfmt);
}

TextHighLighter::~TextHighLighter()
{

}

void TextHighLighter::highlightBlock(const QString &text)
{
    for(QVector<oneline_rule_t>::Iterator it = m_olrules.begin(); it != m_olrules.end(); it++)
    {
        QRegularExpressionMatchIterator matchit = it->first.globalMatch(text);

        while(matchit.hasNext())
        {
            QRegularExpressionMatch match = matchit.next();
            this->setFormat(match.capturedStart(), match.capturedLength(), it->second);
        }
    }

    if(!m_ocrules.open_pattern.pattern().isEmpty())
    {
        this->setCurrentBlockState(0);

        int startIndex = 0;

        if(1 != this->previousBlockState()) // Indicate don't start find the start flag in previous block string
        {
             startIndex = text.indexOf(m_ocrules.open_pattern);
        }

        while(startIndex >= 0)
        {
            int commentLength = 0;

            QRegularExpressionMatch endmatch = m_ocrules.close_pattern.match(text, startIndex);
            int endIndex = endmatch.capturedStart();

            if(-1 == endIndex) // Don't find the end flag
            {
                this->setCurrentBlockState(1); // Indicate don't find the end flag in current block string
                commentLength = text.length() - startIndex;
            }
            else // Found the end flag
            {
                commentLength = endIndex - startIndex
                             + endmatch.capturedLength();
            }

            this->setFormat(startIndex, commentLength, m_ocrules.format);
            startIndex = text.indexOf(m_ocrules.open_pattern, startIndex + commentLength);
        }
    }
}
