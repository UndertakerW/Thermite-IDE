#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "highlighter.h"
#include <QRegularExpression>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void initFileData();

private:
    QColor windowColor;
    QFont textFont;
    Ui::MainWindow *ui;

    //Status
    bool startUp;

    //Process
    QProcess processBuild;
    QProcess processRun;
    QProcess processAssemble;
    QProcess processSimulate;
    QProcess processReadme;

    //File
    QString fileName;
    QString filePath;
    bool fileSaved;
    bool runState;

    //Run
    QString output;
    QString error;

public slots:
    void changeSaveState();

    //Toolbar
    void newFile();
    void saveFile();
    void saveFileAs();
    void openFile();
    void undo();
    void redo();
    void copy();
    void cut();
    void paste();
    void run();
    void selectAll();
    void about();
    void readme();

    /*
    void settings();
    void compile();
    void assemble();
    void simulate();
    */

    void runFinished();
    void updateOutput();
    void updateError();

public:
    void updateInput(QString data);
    void updateWindowTitle();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
};

#endif // MAINWINDOW_H
