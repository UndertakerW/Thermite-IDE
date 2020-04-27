#include "associationlist.h"

/*
 * The constructor of the association widget
 */
AssociationList::AssociationList(QWidget *parent): QListWidget(parent){
    p = (QPlainTextEdit *) parent;
    //Set the color
    toolbarColor.setRgb(248, 248, 248);    //Light Grey
    highlightColor.setRgb(248, 248, 248);  //Light Grey
    QPalette p = this->palette();
    p.setColor(QPalette::Active, QPalette::Base,toolbarColor);
    p.setColor(QPalette::Inactive, QPalette::Base, toolbarColor);
    p.setColor(QPalette::Active, QPalette::Highlight, highlightColor);
    p.setColor(QPalette::Inactive, QPalette::Highlight, highlightColor);
    p.setColor(QPalette::Text,Qt::black);
    this->setPalette(p);
    this->setFont(parent->font());
}

/*
 * Dealing with key press
 */
void AssociationList::keyPressEvent(QKeyEvent *event){
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
