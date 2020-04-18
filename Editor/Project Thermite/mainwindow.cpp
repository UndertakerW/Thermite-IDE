#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QFileDialog>
#include <QFile>
#include <QTextStream>

/*
 * The constructor of MainWindow
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    firstLoad = true;
    ui->setupUi(this);

    //Initialize the status bar
    ui->outputText->parentWindow = this;
    ui->statusBar->showMessage(tr(" Project Thermite Version 0.1"));

    //Initialize the menu bar
    ui->menuBar->setStyleSheet("QMenuBar{background:rgb(248, 248, 248);}");

    //Window color
    QPalette p = this->palette();
    windowColor.setRgb(248,248,248);
    p.setColor(QPalette::Active,QPalette::Window,QColor(windowColor));
    p.setColor(QPalette::Inactive,QPalette::Window,QColor(windowColor));
    this->setPalette(p);

    //Initialize the file data
    initFileData();

    connect(ui->actionNewFile,SIGNAL(triggered(bool)),this,SLOT(newFile()));
    connect(ui->actionOpen,SIGNAL(triggered(bool)),this,SLOT(openFile()));
    connect(ui->actionSaveFile,SIGNAL(triggered(bool)),this,SLOT(saveFile()));
    connect(ui->actionUndo,SIGNAL(triggered(bool)),this,SLOT(undo()));
    connect(ui->actionRedo,SIGNAL(triggered(bool)),this,SLOT(redo()));
    connect(ui->editor,SIGNAL(textChanged()),this,SLOT(changeSaveState()));
    connect(ui->actionRun,SIGNAL(triggered(bool)),this,SLOT(run()));
    connect(&process,SIGNAL(finished(int)),this,SLOT(runFinished(int)));
    connect(&process,SIGNAL(readyReadStandardOutput()),this,SLOT(updateOutput()));
    connect(&process,SIGNAL(readyReadStandardError()),this,SLOT(updateError()));
    connect(ui->actionAbout,SIGNAL(triggered(bool)),this,SLOT(about()));

    fileSaved=true;
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event){
  QMainWindow::resizeEvent(event);
  ui->editor->setGeometry(10,0,width()-20,height()-ui->statusBar->height()-160);
                          //-ui->mainToolBar->height()-80-15);
  ui->outputText->setGeometry(10,ui->editor->height()+10,this->width()-20,115);
    //ui->statusBar->setGeometry(30,ui->editor->height()+ui->outputText->height()+10,this->width()-20,22);
}
void MainWindow::initFileData(){
    fileName = tr("Untitled.cpp");
    filePath = tr("~/Desktop/Untitled.cpp");
    fileSaved = true;
    fileStatus = false;
}
void MainWindow::undo(){
  ui->editor->undo();
}
void MainWindow::redo(){
  ui->editor->redo();
}
void MainWindow::saveFile(){
  QString savePath=QFileDialog::getSaveFileName(this,tr("Choose the path and filename for saving"),fileName,tr("C++ Code File(*.cpp *.c *.h)"));
  if(!savePath.isEmpty()){
      QFile out(savePath);
      out.open(QIODevice::WriteOnly|QIODevice::Text);
      QTextStream str(&out);
      str<<ui->editor->toPlainText();
      out.close();
      fileSaved=true;
      QRegularExpression re(tr("(?<=\\/)\\w+\\.cpp|(?<=\\/)\\w+\\.c|(?<=\\/)\\w+\\.h"));
      fileName=re.match(savePath).captured();
      filePath=savePath;
      this->setWindowTitle(tr("Project Thermite - ")+fileName);
    }
}
void MainWindow::newFile(){
  MainWindow *newWindow=new MainWindow();
  QRect newPos=this->geometry();
  newWindow->setGeometry(newPos.x()+10,newPos.y()+10,newPos.width(),newPos.height());
  newWindow->show();
}
void MainWindow::openFile(){
  if(!fileSaved){
      if(QMessageBox::Save==QMessageBox::question(this,tr("File Unsaved"),tr("The current file is not saved. Do you want to save it?"),QMessageBox::Save,QMessageBox::Cancel))
        saveFile();
    }
  QString openPath=QFileDialog::getOpenFileName(this,tr("Choose the file to open"),filePath,tr("C++ Code File(*.cpp *.c *.h)"));
  if(!openPath.isEmpty()){
      QFile in(openPath);
      in.open(QIODevice::ReadOnly|QIODevice::Text);
      QTextStream str(&in);
      ui->editor->setPlainText(str.readAll());
      QRegularExpression re(tr("(?<=\\/)\\w+\\.cpp|(?<=\\/)\\w+\\.c|(?<=\\/)\\w+\\.h"));
      fileName=re.match(openPath).captured();
      this->setWindowTitle(tr("Project Thermite - ")+fileName);
      filePath=openPath;
      fileSaved=true;
    }
}
void MainWindow::run(){
  if(fileStatus){
      process.terminate();
      return;
    }
  if(!fileSaved){
      if(QMessageBox::Save==QMessageBox::question(this,tr("File Unsaved"),tr("A file must be saved and executed. Do you want to save it?"),QMessageBox::Save,QMessageBox::Cancel))
        saveFile();
    }
  if(fileSaved){
    //if(process!=nullptr)delete process;
    fileStatus=true;
    ui->statusBar->showMessage(tr(" Program running"));
    ui->outputText->clear();
    output.clear();
    error.clear();
    QString buildPath;
    QRegularExpression re(tr(".*(?=\\.cpp)|.*(?=\\.c)|.*(?=\\.h)"));
    buildPath=re.match(filePath).captured();
    //qDebug()<<buildPath;
    process.start("bash", QStringList() << "-c" << QString(tr("g++ ")+filePath+tr(" -o ")+buildPath+tr(";")+buildPath));
    process.waitForStarted();
    ui->outputText->setFocus();
    }
}
void MainWindow::runFinished(int code){
  fileStatus=false;
  qDebug()<<tr("exit code=")<<code;
  ui->statusBar->showMessage(tr(" Project Thermite Version 0.1"));
}
void MainWindow::updateOutput(){
  output=QString::fromLocal8Bit(process.readAllStandardOutput());
  //ui->outputText->setPlainText(output+tr("\n")+error);
  ui->outputText->setPlainText(ui->outputText->toPlainText()+output);//+tr("\n"));
}
void MainWindow::updateError(){
  error=QString::fromLocal8Bit(process.readAllStandardError());
  //ui->outputText->setPlainText(output+tr("\n")+error);
  ui->outputText->setPlainText(ui->outputText->toPlainText()+error);//+tr("\n"));
  process.terminate();
  fileStatus=false;
}
void MainWindow::inputData(QString data){
  if(fileStatus)process.write(data.toLocal8Bit());
}
void MainWindow::closeEvent(QCloseEvent *event){
  if(!fileSaved){
      if(QMessageBox::Save==QMessageBox::question(this,tr("Exit without saving?"),tr("The current file is not saved. Do you want to save it?"),QMessageBox::Save,QMessageBox::Cancel))
        saveFile();
      fileSaved=true;
    }
}
void MainWindow::about(){
  QMessageBox::information(this,tr("About"),tr("Project Thermite Version 0.1 \nBy AAAAgito, Lean-Li, MoyuST, PeterLau61, Sylviachelsea, and UndertakerW @CUHKSZ "),QMessageBox::Ok);
}
