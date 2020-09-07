#include "markdownhighlighter.h"

#include <QRegularExpression>
#include <QDebug>
#include <QTextDocument>

#define STATE_CODE 100

MarkdownHighlighter::MarkdownHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    parent->setDefaultStyleSheet("Table {background-color: red}");

    for (int i = 0; i < 9; ++i) {
        QTextCharFormat f;
        f.setFontWeight(QFont::Bold);
        f.setForeground(Qt::blue);
        f.setFontPointSize(20 - i * 2);
        titleFormats.append(f);
    }

    titleRegex = QRegularExpression("^#+\\s");

    HighlightingRule rule;

    //bold
    QTextCharFormat boldFormat;
    boldFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression(QStringLiteral("\\*\\*.*\\*\\*"));
    rule.format = boldFormat;
    highlightingRules.append(rule);


    //italic
    QTextCharFormat italicFormat;
    italicFormat.setFontItalic(true);
    rule.pattern = QRegularExpression(QStringLiteral("\\_.*\\_"));
    rule.format = italicFormat;
    highlightingRules.append(rule);

    QTextCharFormat linkFormat;
    linkFormat.setForeground(Qt::blue);
    linkFormat.setFontUnderline(true);
    rule.pattern = QRegularExpression(QStringLiteral("https?\\:\\/\\/\\S+"));
    rule.format = linkFormat;
    highlightingRules.append(rule);

    codeFormat.setForeground(Qt::black);
    codeFormat.setBackground(QColor(230, 230, 230));
}

void MarkdownHighlighter::highlightBlock(const QString &text)
{
    if (text == "```") {
        setCurrentBlockState(previousBlockState() == STATE_CODE ? 0
                                                                : STATE_CODE);
        setFormat(0, text.count(), codeFormat);
        return;
    }
    if (previousBlockState() == STATE_CODE) {
        setCurrentBlockState(STATE_CODE);
        setFormat(0, text.count(), codeFormat);
        return;
    }

    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    auto m = titleRegex.match(text);
    if (m.hasMatch()) {
        setFormat(0, text.count(), titleFormats.at(m.captured().length() - 2));
        return;
    }
}
