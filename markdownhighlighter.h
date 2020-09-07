#ifndef MARKDOWNHIGHLIGHTER_H
#define MARKDOWNHIGHLIGHTER_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>

class MarkdownHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    QList<QTextCharFormat> titleFormats;
    QRegularExpression titleRegex;

    QTextCharFormat codeFormat;
    bool inCode{false};

    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;


public:
    MarkdownHighlighter(QTextDocument *parent);

    // QSyntaxHighlighter interface
protected:
    void highlightBlock(const QString &text);
};

#endif // MARKDOWNHIGHLIGHTER_H
