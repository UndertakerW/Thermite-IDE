#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QtWidgets>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <QPlainTextEdit>
#include <QObject>
#include <QPainter>
#include <QListWidget>
#include <QListWidgetItem>
#include "associationlist.h"
#include "highlighter.h"
#include <algorithm>
QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;


class CodeEditor : public QPlainTextEdit{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();



protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void initAssociationDict();
    void placeAssociationList();


private slots:
    void placeLineNumberArea();
    void updateLineNumberArea(const QRect &, int);
    void updateAssociationList();
    void displayAssociationList();


private:
    QWidget * lineNumberArea;
    QFont textFont;
    QColor lineNumberColor;
    QColor backgroundColor;
    QColor textColor;
    QStringList associationDict;
    AssociationList * associationList;
    QString getWordAtCursor();
    int associationState; //Inactive = 0; Active = 1; Hidden = 2;
    Highlighter * highlighter;

};


class LineNumberArea : public QWidget{
public:

    LineNumberArea(CodeEditor * editor);

    QSize sizeHint() const override;

protected:
    //Override the paintEvent function to paint the line numbers
    void paintEvent(QPaintEvent *event) override;

private:
    CodeEditor *codeEditor;

};


#endif
