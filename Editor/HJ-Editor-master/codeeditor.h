#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include <QPainter>
#include <QListWidget>
#include <QListWidgetItem>
#include "associationwidget.h"
#include <algorithm>
QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;

//![codeeditordefinition]

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void initAssociationDict();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateLineNumberAreaWidth();
    void updateLineNumberArea(const QRect &, int);
    void displayAssociationList();

private:
    QWidget * lineNumberArea;
    QFont textFont;
    QColor lineNumberColor;
    QColor backgroundColor;
    QColor textColor;
    QStringList associationDict;
    //QListWidget *completeWidget;
    AssociationWidget * associationWidget;
    QString getWordAtCursor();
    int associationState; //Inactive = 0; Active = 1; Hidden = 2;
    void placeAssociationWidget();
};

//![codeeditordefinition]
//![extraarea]

class LineNumberArea : public QWidget{
public:

    LineNumberArea(CodeEditor * editor);

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    //Override the paintEvent function to paint the line numbers
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};
enum CompleteState{
  Ignore=0,
  Showing=1,
  Hide=2
};

//![extraarea]

#endif
