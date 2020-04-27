#include "terminal.h"

/*
 * The constructor of Terminal
 */
Terminal::Terminal(QWidget* parent): QPlainTextEdit(parent){

    //When the cursor position is changed to some other line, replace the cursor
    connect(this,SIGNAL(cursorPositionChanged()),this,SLOT(replaceCursor()));

    //Set background and text color
    terminalColor.setRgb(255, 255, 255); //White
    textColor.setRgb(0, 0, 0);           //Black
    QPalette p = this->palette();
    p.setColor(QPalette::Active, QPalette::Base, terminalColor);
    p.setColor(QPalette::Inactive, QPalette::Base, terminalColor);
    p.setColor(QPalette::Text, textColor);
    this->setPalette(p);
}

/*
 * Dealing with key press
 */
void Terminal::keyPressEvent(QKeyEvent *event){
    //The current line cannot be deleted
    if(event->key() == Qt::Key_Backspace && this->textCursor().atBlockStart());
    //When a enter or return is input
    else if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        QString data = (this->textCursor()).block().text()+tr("\n");
        qDebug() << "Terminal Input:  " << data;
        mainWindow->updateInput(data);
        //Put the cursor in the end of the line
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        this->setTextCursor(cursor);
    }
    //Call the keypressEvent method of the super class (QPlainTextEdit)
    QPlainTextEdit::keyPressEvent(event);
}

/*
 * In the terminal,
 * the cursor must be always in the current line
 * Therefore, when the cursor goes to some other line
 * need to replace the cursor
 */
void Terminal::replaceCursor(){
    QTextCursor cursor = this->textCursor();
    if (cursor.block().next().isValid()){
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        this->setTextCursor(cursor);
    }
}
