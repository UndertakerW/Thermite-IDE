#ifndef ASSOCIATIONLISTWIDGET_H
#define ASSOCIATIONLISTWIDGET_H

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
class AssociationWidget : public QListWidget
{
public:
  AssociationWidget(QWidget *parent = nullptr);
  static int ldistance(const string source, const string target);
protected:
  void keyPressEvent(QKeyEvent *event) override;
private:
  QPlainTextEdit* p;
  QColor toolbarColor;
  QColor highlightColor;
};

#endif // ASSOCIATIONLISTWIDGET_H
