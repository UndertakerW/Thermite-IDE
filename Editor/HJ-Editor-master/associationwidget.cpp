#include "associationwidget.h"

AssociationWidget::AssociationWidget(QWidget *parent): QListWidget(parent){
  p = (QPlainTextEdit *) parent;
  toolbarColor.setRgb(248, 248, 248);    //Light Grey
  highlightColor.setRgb(248, 248, 248);  //Light Grey
  QPalette palette=this->palette();
  palette.setColor(QPalette::Active,QPalette::Highlight,highlightColor);
  palette.setColor(QPalette::Inactive,QPalette::Highlight,highlightColor);
  palette.setColor(QPalette::Active, QPalette::Base,toolbarColor);
  palette.setColor(QPalette::Inactive, QPalette::Base, toolbarColor);
  palette.setColor(QPalette::Text,Qt::black);
  this->setPalette(palette);
}
void AssociationWidget::keyPressEvent(QKeyEvent *event){
    //When the focus is on the keyword association list
    //Call the keyPressEvent method of the superclass QListWidget when the key press is UP or DOWN
    if(event->key()==Qt::Key_Up || event->key()==Qt::Key_Down){
        QListWidget::keyPressEvent(event);
    }
    //Send the event to the QPlainTextEdit object when the key press is anything else
    //And set the focus back to the QPlainTextEdit object
    else {
        QApplication::sendEvent(p, event);
        p->setFocus();
    }
}
