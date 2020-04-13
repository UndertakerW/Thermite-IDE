#include <QtWidgets>
#include <QDebug>
#include "codeeditor.h"
//![constructor]

/*
 * The constructor of the editor object
 * The parameter parent is a QWidget pointer (to the parent object)
 * The super class is QPlainTextEdit
 */
CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent){

    //Create the area for line numbers
    lineNumberArea = new LineNumberArea(this);

    /* Connect signals to corresponding actions */

    //When the number of lines change, calculate the width of the line number area
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth()));

    //When scrolling the text box, scroll the text
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));

    //When the cursor position changes, show all the widgets
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(displayAssociationList()));

    /* Initialization */

    //Initialize the text box
    textFont = QFont("Courier", 12);
    parent->setFont(textFont);
    updateLineNumberAreaWidth();

    //Set colors by RGB
    lineNumberColor.setRgb(192, 192, 192); //Silver
    backgroundColor.setRgb(255, 255, 255); //White
    textColor.setRgb(0, 0, 0);             //Black

    QPalette p = this->palette();
    p.setColor(QPalette::Active, QPalette::Base, backgroundColor);
    p.setColor(QPalette::Inactive, QPalette::Base, backgroundColor);
    p.setColor(QPalette::Text,textColor);
    this->setPalette(p);



    //Initialize the keyword association function
    initAssociationDict();
    associationWidget = new AssociationWidget(this);
    associationWidget->hide();
    associationWidget->setMaximumHeight(fontMetrics().height()*5);
    associationState = 2;

}

//![constructor]

//![extraAreaWidth]

/*
 * Calculating the width of the line number area
 */
int CodeEditor::lineNumberAreaWidth(){
    //The number of digits of the index (line number)
    int digit = 1;
    int index;
    if (blockCount() >= 1) index = blockCount();
    else index = 1;

    //Count the number of digits
    while (index >= 10) {
        index /= 10;
        digit++;
    }

    //The space to reserve
    int space = fontMetrics().width(QLatin1Char('0')) * (digit + 1);

    return space;
}

//![extraAreaWidth]

//![slotUpdateExtraAreaWidth]

/*
 * Reserve place for the index area
 */
void CodeEditor::updateLineNumberAreaWidth(){
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

//![slotUpdateExtraAreaWidth]

//![slotUpdateRequest]
/*
 * On scrolling the text box in the vertical direction, scroll the line numbers & text
 * The parameter &rect is a reference to a QRect object (the window)
 * deltaY is the change in the vertical direction (how much the user scrolls)
 */
void CodeEditor::updateLineNumberArea(const QRect &rect, int deltaY)
{
    if (deltaY)
        lineNumberArea->scroll(0, deltaY);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth();

}

//![slotUpdateRequest]

//![resizeEvent]
/*
 * Adjust the size of the window
 */
void CodeEditor::resizeEvent(QResizeEvent *e){

    //Call the resizeEvent method of the super class
    QPlainTextEdit::resizeEvent(e);

    //Adjust the editor according to the new window
    QRect rect = contentsRect();
    lineNumberArea->setGeometry(QRect(rect.left(), rect.top(), lineNumberAreaWidth(), rect.height()));
}

//![resizeEvent]

//![extraAreaPaintEvent_0]
/*
 * Print the line numbers
 */
void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event){

    //Instantiate the QPainter class
    QPainter painter(lineNumberArea);

    //Before paint the line numbers, paint the line number area
    painter.fillRect(event->rect(), backgroundColor);

//![extraAreaPaintEvent_0]

//![extraAreaPaintEvent_1]
    QTextBlock block = firstVisibleBlock();

    //blockNumber starts from 0, but line number starts from 1
    int lineNumber = block.blockNumber() + 1;

    //The position of the top of the block
    int top = int (blockBoundingGeometry(block).translated(contentOffset()).top());
    int height = int (blockBoundingRect(block).height());
//![extraAreaPaintEvent_1]

//![extraAreaPaintEvent_2]
    while (block.isValid()) {

        //Draw the current line number
        painter.setPen(lineNumberColor);
        painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignCenter,
                         QString::number(lineNumber));

        //The position of the top of the current block + The height of the block
        //= the position of the bottom of the block = the position of the top of the next block
        top += height;

        //Go to the next block
        block = block.next();
        lineNumber++;
    }
}
//![extraAreaPaintEvent_2]

/*
 * Dealing with key press
 */
void CodeEditor::keyPressEvent(QKeyEvent *event){

    //Keyword association

    //Move down the option
    if (associationState == 1 && event->key() == Qt::Key_Down){
        int index = associationWidget->currentRow();
        int count = associationWidget->count();
        if (index < count-1){
            associationWidget->setCurrentRow(index+1);
        }
    }

    //Move up the option
    else if(associationState == 1 && event->key() == Qt::Key_Up){
        int index = associationWidget->currentRow();
        if(index > 0)
            associationWidget->setCurrentRow(index-1);
    }

    //Select the current option
    else if(associationState == 1 && event->key()==Qt::Key_Return){
        QString insertText = associationWidget->currentItem()->text();
        QString word = this->getWordAtCursor();

        //Shut down the association list
        associationState = 0;

        //Delete the incomplete word and insert the selected keyword
        for(int i=0; i<word.count(); i++)
                this->textCursor().deletePreviousChar();
        this->insertPlainText(insertText);


        if(insertText == tr("#include <>") || insertText == tr("#include \"\""))
            this->moveCursor(QTextCursor::PreviousCharacter);

        associationState = 2;
        associationWidget->hide();
    }

    //Autocomplete parentheses
    else if(event->key() == '('){
        this->insertPlainText(tr("()"));
        //Move the cursor to inbetween the parentheses
        this->moveCursor(QTextCursor::PreviousCharacter);
    }

    //Autocomplete square brackets
    else if(event->key() == '['){
        this->insertPlainText(tr("[]"));
        //Move the cursor to inbetween the square brackets
        this->moveCursor(QTextCursor::PreviousCharacter);
    }

    //Autocomplete double quotation mark
    else if(event->key() == '"'){
        this->insertPlainText(tr("\"\""));
        //Move the cursor to inbetween the quotation marks
        this->moveCursor(QTextCursor::PreviousCharacter);
    }

    //Auto indentation
    else if(event->key()==Qt::Key_Return){

    //Get the text of the current line
    QString line = this->document()->findBlockByLineNumber(this->textCursor().blockNumber()).text();

    //Call the keyPressEvent method of the superclass QPlainTextEdit
    QPlainTextEdit::keyPressEvent(event);

    //Do nothing if the current line is empty
    if(line.count() == 0) return;

    //Output the indentation of the current line
    int empty = 0;
        for(QChar c : line){
            if(c.isSpace()){
                this->insertPlainText(QString(QChar::Space));
                empty++;
            }
            else if (c == tr("\t")){
                this->insertPlainText(tr("  "));
                empty++;
            }
            else break;
        }

    //Indentation increase
    //The current line ends with ')'
    QString lineContent = line.right(line.count()-empty);
        if(line.at(line.count()-1)==')'){
            if (lineContent.startsWith(tr("for(")) || lineContent.startsWith(tr("while(")) ||
                    lineContent.startsWith(tr("switch(")) || lineContent.startsWith(tr("if("))){
                for(QChar c : line){
                    if(c.isSpace()){
                        this->insertPlainText(QString(QChar::Space));
                    }
                    else if (c == tr("\t")){
                        this->insertPlainText(tr("  "));
                    }
                    else break;
                }
                this->insertPlainText(tr("  "));
            }
        }
    //The current line ends with '{'
        if(line.at(line.count()-1) == '{'){
            this->insertPlainText(tr("  "));
            int pos = this->textCursor().position();
            this->insertPlainText(tr("\n"));
            for(QChar c : line){
                if(c.isSpace()){
                    this->insertPlainText(QString(QChar::Space));
                }
                else if (c == tr("\t")){
                    this->insertPlainText(tr("  "));
                }
                else break;
            }
            this->insertPlainText(tr("}"));
            QTextCursor cursor = this->textCursor();
            cursor.setPosition(pos);
            this->setTextCursor(cursor);
            }
        }

    else if(event->key() == Qt::Key_Backspace){

        QChar prev = QChar(this->document()->characterAt(this->textCursor().position()-1));
        QChar curr = QChar(this->document()->characterAt(this->textCursor().position()));

        //Pairwise deletion
        if (prev == '('){
            if(curr == ')')
                this->textCursor().deleteChar();
        }
        else if (prev == '['){
            if(curr == ']')
                this->textCursor().deleteChar();
        }
        else if (prev == '<'){
            if(curr == '>')
                this->textCursor().deleteChar();
        }
        else if (prev == '\"'){
            if(curr == '\"')
                this->textCursor().deleteChar();
        }
        QPlainTextEdit::keyPressEvent(event);
    }

    else{
        QPlainTextEdit::keyPressEvent(event);
    }
}

void CodeEditor::initAssociationDict(){
  associationDict << "char" << "class" << "const"
              << "double" << "enum" << "explicit"
              << "friend" << "inline" << "int"
              << "long" << "namespace" << "operator"
              << "private" << "protected" << "public"
              << "short" << "signals" << "signed"
              << "slots" << "static" << "struct"
              << "template" << "typedef" << "typename"
              << "union" << "unsigned" << "virtual"
              << "void" << "volatile" << "bool"<<"using"<<"constexpr"
              <<"sizeof"<<"if"<<"for"<<"foreach"<<"while"<<"do"<<"case"
              <<"break"<<"continue"<<"template"<<"delete"<<"new"
              <<"default"<<"try"<<"return"<<"throw"<<"catch"<<"goto"<<"else"
              <<"extren"<<"this"<<"switch"<<"#include <>"<<"#include \"\""<<"#define"<<"iostream";
}

/*
 * Get the word pointed by the cursor
 */
QString CodeEditor::getWordAtCursor(){
    int pos = this->textCursor().position();
    QVector<QChar> chs;
    QString word;

    //If there is any digit/ letter/ whitespace after the cursor, return nothing
    QChar ch = this->document()->characterAt(pos);
    if(ch.isDigit() || ch.isLetter() || ch==QChar::Space)
        return word;

    //Push any digit/ letter/ underscope/ hash before the cursor into the vector chs
    //Until reaching something else
    ch = this->document()->characterAt(--pos);
    while (ch.isDigit() || ch.isLetter() || ch=='_' || ch=='#'){
        chs.append(ch);
        ch = this->document()->characterAt(--pos);
    }
    if(chs.size() == 0){
        return word;
    }
    else{
        for(int i = chs.size()-1; i >= 0; i--)
            word += chs[i];
        return word;
    }
}

void CodeEditor::displayAssociationList(){
    //Temporarily shut down the association widget to ignore changes of cursor and/or text
    //This prevents an infinite loop and mutual restraint
    if(associationState == 0) return;

    //Firstly hide the widget
    associationWidget->hide();
    associationState = 2;

    //Get the word pointed by the cursor
    QString word = this->getWordAtCursor();

    //Clear the association list (the last list)
    //The clear() method will delete the "new QListWidgetItem(keyword)" in the heap
    //So no need to delete them again
    associationWidget->clear();

    if(word.length() >= 2){
        int width = 0;
        vector<QString> associationList;
        for(const QString &keyword : associationDict){
            if(keyword.startsWith(word) && keyword != word){
                associationList.push_back(keyword);
                if(keyword.length() > width)
                    width = keyword.length();
            }
        }

        //Sort the association list by the length of the keyword.
        if(associationList.size() > 0){
            for(unsigned int i = 0; i < associationList.size()-1; i++){
                for(unsigned int j = 0; j < associationList.size()-1-i; j++){
                    if(associationList.at(j).length() > associationList.at(j+1).length()){
                        QString temp = associationList.at(j);
                        associationList.at(j) = associationList.at(j+1);
                        associationList.at(j+1) = temp;
                  }
              }
          }

        //Put the keywords in the association list
        for(const QString& keyword : associationList){
            associationWidget->addItem(new QListWidgetItem(keyword));
        }

        //int x = this->getAssociationWidgetWidth();
        //int y = this->cursorRect().y()+fontMetrics().height();

        //associationWidget->move(x,y);

        placeAssociationWidget();

        if(associationWidget->count() >= 4)
            associationWidget->setFixedHeight(fontMetrics().height()*4);
        else
            associationWidget->setFixedHeight(fontMetrics().height()*(associationWidget->count()+1));

        associationWidget->setFixedWidth((fontMetrics().width(QLatin1Char('0')))*width*2);

        associationWidget->show();

        associationState = 1;

        associationWidget->setCurrentRow(0,QItemSelectionModel::Select);

        }
    }
}

void CodeEditor::placeAssociationWidget(){
    QTextCursor cursor=this->textCursor();
    int pos=cursor.position()-1;
    int origianlPos=pos+1;
    QChar ch;
    ch=this->document()->characterAt(pos);
    while((ch.isDigit()||ch.isLetter()||ch=='_'||ch=='#')&&pos>0){
        pos--;
        ch=this->document()->characterAt(pos);
      }
    pos++;
    associationState = 0;
    cursor.setPosition(pos);
    this->setTextCursor(cursor);
    int x=this->cursorRect().x()+2*fontMetrics().width(QLatin1Char('0'));
    cursor.setPosition(origianlPos);
    this->setTextCursor(cursor);
    associationState = 2;
    int y = this->cursorRect().y()+fontMetrics().height();
    associationWidget->move(x,y);
}


LineNumberArea::LineNumberArea(CodeEditor * editor) : QWidget(editor) {
    codeEditor = editor;
}
