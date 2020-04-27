#include "highlighter.h"

/*
 * The constructor of the highlighter
 */
Highlighter::Highlighter(QTextDocument * parent): QSyntaxHighlighter(parent){
    //Highlight colors
    keywordColor.setRgb(30, 144, 255);  //DoderBlue
    functionColor.setRgb(255, 20, 147); //DeepPink
    classColor.setRgb(255, 20, 147);    //DeepPink
    commentColor.setRgb(60, 179, 113);  //SpringGreen
    includeColor.setRgb(60, 179, 113);  //SpringGreen

    keywordFormat.setForeground(keywordColor);
    functionFormat.setForeground(functionColor);
    classFormat.setForeground(QColor(classColor));
    singleLineCommentFormat.setForeground(commentColor);
    multiLineCommentFormat.setForeground(commentColor);
    includeFormat.setForeground(includeColor);


    //Keyword
    initKeywordPatternsDict();
    HighlightRule rule;
    for (const QString &pattern : keywordPatternsDict){
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        HighlightRules.append(rule);
    }

    //Function
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    HighlightRules.append(rule);

    //Class
    rule.pattern = QRegularExpression("(?<=class\\s)\\w*");
    rule.format = classFormat;
    HighlightRules.append(rule);

    //Single-Line Comments
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    HighlightRules.append(rule);

    //Multiple-Line Comments
    //Special handling in the overriden highlightBlock method

    //Include
    rule.pattern = QRegularExpression("(?<=#include\\s)(<.*>)|(?<=#include)(<.*>)|(?<=#include\\s)(\".*\")|(?<=#include)(\".*\")|\".*\"");
    rule.format = includeFormat;
    HighlightRules.append(rule);
}

/*
 * Override the highlightBlock method
 * To highlight the text
 */
void Highlighter::highlightBlock(const QString &text){
    //Search the text for each highlight rule
    for (const HighlightRule &rule : HighlightRules){
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        //If found, highlight the text according to the format
        while (matchIterator.hasNext()){
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    //Detection for multiple-line comments

    //Block State:
    //0: not in comment, 1: in comment
    //Set the state of the current block to be 0 (not in comment)
    setCurrentBlockState(0);

    //Search for /* if the state of the previous block is not 1 (the previous block is not in comment)
    int start = 0;
    if (previousBlockState() != 1)
        start = text.indexOf(QRegularExpression("/\\*"));

    //If found
    while (start >= 0) {
        //Search for the end of this multiple-line comment
        QRegularExpressionMatch match = QRegularExpression("\\*/").match(text, start);
        int end = match.capturedStart();
        int length = 0;

        //If this multiple-line comment has no end
        if (end == -1) {
            //Set the state of the current block to be 1 (in comment)
            setCurrentBlockState(1);
            length = text.length() - start;
        }

        //Else if this multiple-line comment has an end
        //The state of the current block will not be changed for the time being
        else {
            length = (end + match.capturedLength()) - start;
        }

        //Highlight this multiple-line comment
        setFormat(start, length, multiLineCommentFormat);

        //Search for the next multiple-line comment
        start = text.indexOf(QRegularExpression("/\\*"), start + length);
    }
}

/*
 * Initialize the keyword patterns dictionary
 */
void Highlighter::initKeywordPatternsDict(){
    std::ifstream in("KeywordPatternsDict.txt");
    std::string pattern;
    while(getline(in, pattern)){
        QString qString = QString::fromStdString(pattern);
        keywordPatternsDict << qString;
    }
    in.close();
}
