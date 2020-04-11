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
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));

    //When scrolling the text box, scroll the text
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));

    //When the cursor position changes, show all the widgets
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(showCompleteWidget()));

    /* Initialization */

    //Initialize the text box
    textFont = QFont("Courier", 12);
    parent->setFont(textFont);
    updateLineNumberAreaWidth();

    //Set colors by RGB
    lineNumberColor.setRgb(192, 192, 192); //Silver
    backgroundColor.setRgb(248, 248, 255); //GhostWhite
    textColor.setRgb(0, 0, 0);             //Black

    QPalette p = this->palette();
    p.setColor(QPalette::Active, QPalette::Base, backgroundColor);
    p.setColor(QPalette::Inactive, QPalette::Base, backgroundColor);
    p.setColor(QPalette::Text,textColor);
    this->setPalette(p);



    //Initialize the keyword list
    setUpCompleteList();

    //Initialize the widgets
    completeWidget = new CompleteListWidget(this);
    completeWidget->hide();
    completeWidget->setMaximumHeight(fontMetrics().height()*5);
    completeState = CompleteState::Hide;
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
    int space = fontMetrics().width(QLatin1Char(12)) * (digit + 2);

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
  //qDebug()<<event->key();
  if(event->modifiers()==Qt::ShiftModifier&&event->key()==40){
      this->insertPlainText(tr("()"));
      this->moveCursor(QTextCursor::PreviousCharacter);
    }
  else if(event->modifiers()==Qt::ShiftModifier&&event->key()==34){
      this->insertPlainText(tr("\"\""));
      this->moveCursor(QTextCursor::PreviousCharacter);
    }
  else if(event->key()==16777235&&completeState==CompleteState::Showing){
      if(completeWidget->currentRow()>0)
        completeWidget->setCurrentRow(completeWidget->currentRow()-1);
    }
  else if(event->key()==16777237&&(completeState==CompleteState::Showing)){
      if(completeWidget->currentRow()<completeWidget->count()-1)
        completeWidget->setCurrentRow(completeWidget->currentRow()+1);
    }
  else if(event->key()==Qt::Key_Return&&(completeState==CompleteState::Showing)){
      QString insertText=completeWidget->currentItem()->text();
      QString word=this->getWordOfCursor();
      completeState=CompleteState::Ignore;
      for(int i=0;i<word.count();++i)
        this->textCursor().deletePreviousChar();
      this->insertPlainText(insertText);
      if(insertText.contains(tr("#include")))
        this->moveCursor(QTextCursor::PreviousCharacter);
      completeState=CompleteState::Hide;
      completeWidget->hide();
    }//*
  else if(event->key()==Qt::Key_Return){//回车下行层级自动缩进功能
      //获得本行的文本
      QString temp=this->document()->findBlockByLineNumber(this->textCursor().blockNumber()).text();
      QPlainTextEdit::keyPressEvent(event);
      if(temp.count()<=0)return;
      //输出回车那一行的前距
      foreach(const QChar &c,temp){
          if(c.isSpace())this->insertPlainText(c);
          else break;
        }
      //如果是for() while() switch() if()则缩进一个tab,一种粗略地做法可能会出错
      if(temp.at(temp.count()-1)==')'&&(temp.contains(tr("for("))||temp.contains(tr("while("))
                                        ||temp.contains(tr("switch("))||temp.contains(tr("if("))))
          this->insertPlainText(tr("\t"));
      //如果是{ 则缩进并补}
      if(temp.at(temp.count()-1)=='{'){
          this->insertPlainText(tr("\t"));
          QTextCursor cursor=this->textCursor();
          int pos=this->textCursor().position();
          this->insertPlainText(tr("\n"));
          foreach(const QChar &c,temp){
              if(c.isSpace())this->insertPlainText(c);
              else break;
            }
          this->insertPlainText(tr("}"));
          cursor.setPosition(pos);
          this->setTextCursor(cursor);//返回中间一行
        }
    }//*/
  else if(event->key()==Qt::Key_Backspace){
      switch(this->document()->characterAt(this->textCursor().position()-1).toLatin1()){
        case '(':
          QPlainTextEdit::keyPressEvent(event);
          if(this->document()->characterAt(this->textCursor().position())==')'){
              this->textCursor().deleteChar();
            }break;
        case '\"':
          QPlainTextEdit::keyPressEvent(event);
          if(this->document()->characterAt(this->textCursor().position())=='\"'){
              this->textCursor().deleteChar();
            }break;
        case '<':
          QPlainTextEdit::keyPressEvent(event);
          if(this->document()->characterAt(this->textCursor().position())=='>'){
              this->textCursor().deleteChar();
            }break;
        default:
          QPlainTextEdit::keyPressEvent(event);
        }
    }
  else{
    QPlainTextEdit::keyPressEvent(event);
    //qDebug()<<event->key();
    }
}
void CodeEditor::setUpCompleteList(){
  completeList<< "char" << "class" << "const"
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
//得到当前光标位置的字符串
QString CodeEditor::getWordOfCursor(){
  int pos=this->textCursor().position()-1;
  QVector<QChar> words;
  QString result;
  QChar ch=this->document()->characterAt(pos+1);
  if(ch.isDigit()||ch.isLetter()||ch==' ')return result;
  ch=this->document()->characterAt(pos);
  if(ch==' ')return result;
  while(ch.isDigit()||ch.isLetter()||ch=='_'||ch=='#'){
      words.append(ch);
      pos--;
      ch=this->document()->characterAt(pos);
    }
  for(int i=words.size()-1;i>=0;i--)
    result+=words[i];
  return result;

}

void CodeEditor::showCompleteWidget(){
  if(completeState==CompleteState::Ignore)return;//忽略光标和文本变化的响应,避免陷入事件死循环和互相钳制
  completeWidget->hide();
  completeState=CompleteState::Hide;
  QString word=this->getWordOfCursor();
  completeWidget->clear();
  if(!word.isEmpty()){//光标所在单词是不是合法(能不能联想)
      int maxSize=0;
      QMap<QString,int> distance;
      vector<QString> itemList;
      foreach(const QString &temp,completeList){
          if(temp.contains(word)){
              //completeWidget->addItem(new QListWidgetItem(temp));
              itemList.push_back(temp);
              distance[temp]=CompleteListWidget::ldistance(temp.toStdString(),word.toStdString());
              if(temp.length()>maxSize)maxSize=temp.length();

            }
        }
      //有没有匹配的字符
      if(itemList.size()>0){//如果有的话
      sort(itemList.begin(),itemList.end(),[&](const QString &s1,const QString &s2)->bool{return distance[s1]<distance[s2]; });
      foreach(const QString& item,itemList){
          completeWidget->addItem(new QListWidgetItem(item));
        }

      int x=this->getCompleteWidgetX();
      int y=this->cursorRect().y()+fontMetrics().height();

      completeWidget->move(x,y);
      if(completeWidget->count()>5)completeWidget->setFixedHeight(fontMetrics().height()*6);
      else completeWidget->setFixedHeight(fontMetrics().height()*(completeWidget->count()+1));
      completeWidget->setFixedWidth((fontMetrics().width(QLatin1Char(12))+6)*maxSize);
      completeWidget->show();
      completeState=CompleteState::Showing;
      completeWidget->setCurrentRow(0,QItemSelectionModel::Select);
        }
    }

}
int CodeEditor::getCompleteWidgetX(){
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
  completeState=CompleteState::Ignore;
  cursor.setPosition(pos);
  this->setTextCursor(cursor);
  int x=this->cursorRect().x()+2*fontMetrics().width(QLatin1Char(12));
  cursor.setPosition(origianlPos);
  this->setTextCursor(cursor);
  completeState=CompleteState::Hide;
  return x;
}

LineNumberArea::LineNumberArea(CodeEditor * editor) : QWidget(editor) {
    codeEditor = editor;
}
