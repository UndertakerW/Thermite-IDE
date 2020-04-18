#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QWidget>
#include <QPlainTextEdit>
#include <QDebug>
#include <QTextBlock>
#include "mainwindow.h"
class Console : public QPlainTextEdit
{
  Q_OBJECT
public:
  Console(QWidget* parent = nullptr);
  MainWindow *parentWindow;
protected:
  void keyPressEvent(QKeyEvent *event)override;
protected slots:
  void resetCursorPosition();
private:
  QColor consoleColor;
};

#endif // CONSOLE_H
