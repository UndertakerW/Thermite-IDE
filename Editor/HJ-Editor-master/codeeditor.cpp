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

    //When the cursor position changes, highlight the current line
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    //When the cursor position changes, show all the widgets
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(showCompleteWidget()));

    /* Initialization */

    //Initialize the text box
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    //Set colors by RGB
    lineColor.setRgb(56,60,69);
    editorColor.setRgb(34,39,49);
    QPalette p = this->palette();
    p.setColor(QPalette::Active, QPalette::Base, editorColor);
    p.setColor(QPalette::Inactive, QPalette::Base, editorColor);
    p.setColor(QPalette::Text,Qt::white);
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

    //Minimum width: 2 digits
    if(digit <= 2) digit = 2;

    //The space to reserve
    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digit;

    return space;
}

//![extraAreaWidth]

//![slotUpdateExtraAreaWidth]

/*
 * Reserve place for the index area
 */
void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

//![slotUpdateExtraAreaWidth]

//![slotUpdateRequest]
/*
 * On scrolling the text box in the vertical direction, scroll the line numbers & text
 * The parameter &rect is a reference to a QRect object (the window)
 * deltaY is the change in the vertical direction (how much the user scrolls)
 */
void CodeEditor::updateLineNumberArea(const QRect &qrect, int deltaY)
{
    if (deltaY)
        lineNumberArea->scroll(0, deltaY);
    else
        lineNumberArea->update(0, qrect.y(), lineNumberArea->width(), qrect.height());

    if (qrect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);

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
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

//![resizeEvent]

//![cursorPositionChanged]
/*
 * Highlight the current line for a better visual effect
 */
void CodeEditor::highlightCurrentLine(){
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::red).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

//![cursorPositionChanged]

//![extraAreaPaintEvent_0]
//打印行号，此函数被paintEvent 调用，paintEvent在头文件里被重写
void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), lineColor);

//![extraAreaPaintEvent_0]

//![extraAreaPaintEvent_1]
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
//![extraAreaPaintEvent_1]

//![extraAreaPaintEvent_2]
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::lightGray);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignCenter, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
//![extraAreaPaintEvent_2]

//自动补全
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
      completeWidget->setFixedWidth((fontMetrics().width(QLatin1Char('9'))+6)*maxSize);
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
  int x=this->cursorRect().x()+2*fontMetrics().width(QLatin1Char('9'));
  cursor.setPosition(origianlPos);
  this->setTextCursor(cursor);
  completeState=CompleteState::Hide;
  return x;
}
