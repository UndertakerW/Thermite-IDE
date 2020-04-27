#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <fstream>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE


class Highlighter : public QSyntaxHighlighter{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;
    void initKeywordPatternsDict();

private:
    QColor keywordColor;
    QColor functionColor;
    QColor classColor;
    QColor commentColor;
    QColor includeColor;
    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat includeFormat;
    QTextCharFormat functionFormat;
    QStringList keywordPatternsDict;
    struct HighlightRule{
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightRule> HighlightRules;
};


#endif // HIGHLIGHTER_H
