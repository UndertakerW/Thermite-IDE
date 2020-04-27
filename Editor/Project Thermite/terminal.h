#ifndef TERMINAL_H
#define TERMINAL_H

#include <QObject>
#include <QWidget>
#include <QPlainTextEdit>
#include <QDebug>
#include <QTextBlock>
#include "mainwindow.h"

class Terminal : public QPlainTextEdit{
    Q_OBJECT

public:
    Terminal(QWidget* parent = nullptr);
    MainWindow *mainWindow;

protected:
    void keyPressEvent(QKeyEvent *event) override;

protected slots:
    void replaceCursor();

private:
    QColor terminalColor;
    QColor textColor;
};

#endif // TERMINAL_H
