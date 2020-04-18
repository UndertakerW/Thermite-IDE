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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void initFileData();

private:
    QColor windowColor;
  QIcon runIcon;
  QIcon stopIcon;
  Ui::MainWindow *ui;
  QProcess process;

  //File
  QString fileName;
  QString filePath;
  bool fileSaved;
  bool fileStatus;

  bool firstLoad;


  //Run
  QString output;
  QString error;

  QFont textFont;

public slots:
  void changeSaveState(){
    //qDebug()<<"changed";
    if(firstLoad&&fileSaved){
        this->setWindowTitle(tr("Project Thermite - ")+fileName);
        firstLoad=false;
        return;
      }
    fileSaved=false;
    this->setWindowTitle(tr("Project Thermite - ")+fileName+tr("*"));
  }

  //Toolbar
  void newFile();
  void saveFile();
  void openFile();
  void undo();
  void redo();
  void run();


  void runFinished(int code);
  void updateOutput();
  void updateError();
  void about();
public:
  void inputData(QString data);
protected:
  void resizeEvent(QResizeEvent* event)override;
  void closeEvent(QCloseEvent* event)override;
};

#endif // MAINWINDOW_H
