#ifndef ASSOCIATIONLIST_H
#define ASSOCIATIONLIST_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QApplication>
#include <vector>
#include <string>
#include <cmath>

using std::string;
using std::vector;
using std::min;

class AssociationList : public QListWidget{

public:
    AssociationList(QWidget *parent);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QPlainTextEdit* p;
    QColor toolbarColor;
    QColor highlightColor;
};

#endif // ASSOCIATIONLIST_H
