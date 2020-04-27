#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

/*
 * The constructor of MainWindow
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    //Initialize the UI
    startUp = 1;
    ui->setupUi(this);
    this->setWindowTitle(tr("Project Thermite"));

    //Initialize the terminal
    ui->terminal->mainWindow = this;

    //Initialize the status bar
    ui->statusBar->showMessage(tr(" Project Thermite Version 0.7"));

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

    //File signals
    connect(ui->actionNewFile,SIGNAL(triggered(bool)),this,SLOT(newFile()));
    connect(ui->actionOpen,SIGNAL(triggered(bool)),this,SLOT(openFile()));
    connect(ui->actionSaveFile,SIGNAL(triggered(bool)),this,SLOT(saveFile()));
    connect(ui->actionSaveAs,SIGNAL(triggered(bool)),this,SLOT(saveFileAs()));

    //Edit signals
    connect(ui->actionUndo,SIGNAL(triggered(bool)),this,SLOT(undo()));
    connect(ui->actionRedo,SIGNAL(triggered(bool)),this,SLOT(redo()));
    connect(ui->actionCopy,SIGNAL(triggered(bool)),this,SLOT(copy()));
    connect(ui->actionCut,SIGNAL(triggered(bool)),this,SLOT(cut()));
    connect(ui->actionPaste,SIGNAL(triggered(bool)),this,SLOT(paste()));
    connect(ui->actionSelectAll,SIGNAL(triggered(bool)),this,SLOT(selectAll()));

    //Help signals
    connect(ui->actionAbout,SIGNAL(triggered(bool)),this,SLOT(about()));
    connect(ui->actionReadme,SIGNAL(triggered(bool)),this,SLOT(readme()));

    //Run signals
    connect(ui->actionRun,SIGNAL(triggered(bool)),this,SLOT(run()));
    connect(&processRun,SIGNAL(finished(int)),this,SLOT(runFinished()));
    connect(&processRun,SIGNAL(readyReadStandardOutput()),this,SLOT(updateOutput()));
    connect(&processRun,SIGNAL(readyReadStandardError()),this,SLOT(updateError()));

    //Status signals
    connect(ui->editor,SIGNAL(textChanged()),this,SLOT(changeSaveState()));
}

/*
 * The destructor of MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Override the resizeEvent method
 */
void MainWindow::resizeEvent(QResizeEvent *event){
    //Call resizeEvent method of the superclass (QMainWindow)
    QMainWindow::resizeEvent(event);

    //Replace the editor
    ui->editor->setGeometry(10,0,width()-20,(height()-ui->statusBar->height())*0.65);

    //Replace the terminal
    ui->terminal->setGeometry(10,ui->editor->height()+10,width()-20,height()-ui->statusBar->height()-ui->editor->height()-45);
}

/*
 * Override the closeEvent method
 */
void MainWindow::closeEvent(QCloseEvent *event){
    if(!fileSaved){
        if(QMessageBox::Save==QMessageBox::question(this,tr("Exit without Saving?"),
                                                    tr("The current file is not saved. Do you want to save it?"), QMessageBox::Save, QMessageBox::Cancel))
            saveFile();
    }
    //Call closeEvent method of the superclass (QMainWindow)
    QMainWindow::closeEvent(event);
}

/*
 * Initialize the file data
 */
void MainWindow::initFileData(){
    fileName = tr("Untitled.cpp");
    filePath = tr("~/Desktop/Untitled.cpp");
    fileSaved = true;
    runState = 0;
}

/*
 * Create a new file
 * Implemented by opening a new editor
 */
void MainWindow::newFile(){
    //Initialize a new instance of the editor
    MainWindow *newWindow = new MainWindow();

    //Place the new editor near the old one
    QRect newPos = this->geometry();
    newWindow->setGeometry(newPos.x()+20,newPos.y()+20,newPos.width(),newPos.height());
    newWindow->show();
}

/*
 * Update the window title when the filename is changed
 */
void MainWindow::updateWindowTitle(){
    //Get the file name by matching
    QRegularExpression pattern(tr("(?<=\\/)\\w+\\.cpp|(?<=\\/)\\w+\\.c|(?<=\\/)\\wq+\\.h"));
    fileName = pattern.match(filePath).captured();
    this->setWindowTitle(fileName + tr(" - Project Thermite"));
}

/*
 * Open a file
 */
void MainWindow::openFile(){
    //If the current file is not saved, ask the user to save
    if(!fileSaved){
        if(QMessageBox::question(this,tr("File Unsaved"),tr("The current file is not saved. Do you want to save it?"),
                                 QMessageBox::Save, QMessageBox::Cancel) == QMessageBox::Save)
            saveFile();
    }
    //When opening a file for the first time, the initial path will be the default path
    //Otherwise, the initial path will be the last filepath
    QString openPath = QFileDialog::getOpenFileName(this,tr("Choose the File to Open"),
                                                    filePath, tr("C++ Code File(*.c *.cpp *.h)"));
    if(!openPath.isEmpty()){
        //Update the filepath
        filePath = openPath;
        //Open the file and put the code in the editor
        QFile in(openPath);
        in.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream code(&in);
        ui->editor->setPlainText(code.readAll());
        in.close();
        fileSaved = true;
        //Update the window title
        updateWindowTitle();
    }
}

/*
 * Save the file
 */
void MainWindow::saveFile(){
    if (filePath == tr("~/Desktop/Untitled.cpp"))
        saveFileAs();
    else{
        QString savePath = filePath;
        //Save the file
        QFile out(savePath);
        out.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream code(&out);
        code << ui->editor->toPlainText();
        out.close();
        fileSaved = true;
        //Update the window title
        updateWindowTitle();
    }
}

/*
 * Save the file as
 */
void MainWindow::saveFileAs(){
    QString savePath = QFileDialog::getSaveFileName(this,tr("Choose the Path and Filename to Save"),
                                                      fileName, tr("C++ Code File(*.c *.cpp *.h)"));
    if(!savePath.isEmpty()){
        //Update the filepath and status
        filePath = savePath;
        //Save the file
        QFile out(savePath);
        out.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream code(&out);
        code << ui->editor->toPlainText();
        out.close();
        fileSaved = true;
        //Update the window title
        updateWindowTitle();
    }
}

/*
 * Undo
 */
void MainWindow::undo(){
    ui->editor->undo();
}

/*
 * Redo
 */
void MainWindow::redo(){
    ui->editor->redo();
}

/*
 * Copy
 */
void MainWindow::copy(){
    ui->editor->copy();
}

/*
 * Cut
 */
void MainWindow::cut(){
    ui->editor->cut();
}

/*
 * Paste
 */
void MainWindow::paste(){
    ui->editor->paste();
}

/*
 * Select all text
 */
void MainWindow::selectAll(){
    ui->editor->selectAll();
}

/*
 * Run the code (using MinGW)
 */
void MainWindow::run(){
    //If already running, terminate the process
    if(runState){
        processBuild.terminate();
        processRun.terminate();
        runState = 0;
        return;
    }
    //If file not saved, ask for saving
    if(!fileSaved){
        if(QMessageBox::question(this,tr("File Unsaved"),tr("A file must be saved and executed. Do you want to save it?"),
                                QMessageBox::Save, QMessageBox::Cancel) == QMessageBox::Save)
            saveFile();
    }
    //If file saved, start the process
    if(fileSaved){
        runState = 1;
        ui->statusBar->showMessage(tr(" Program running"));
        //Clear the terminal
        ui->terminal->clear();
        output.clear();
        error.clear();
        //Get the path for building
        QRegularExpression pattern(tr(".*(?=\\.c)|.*(?=\\.cpp)|.*(?=\\.h)"));
        QString buildPath = pattern.match(filePath).captured();
        QString build = QString(tr("g++ ") + filePath + tr(" -o ") + buildPath);
        //Compile and Run
        processBuild.start(build);
        processBuild.waitForStarted();
        processBuild.waitForFinished();
        processRun.start(buildPath);
        processRun.waitForStarted();
        ui->terminal->setFocus();
    }
}

/*
 * Whenever the code is changed
 * Change the fileSaved state to 0
 */
void MainWindow::changeSaveState(){
    fileSaved = 0;
    this->setWindowTitle(fileName + tr("*") + tr(" - Project Thermite"));
}

/*
 * When the run finishes
 * Change the runState to 0
 */
void MainWindow::runFinished(){
    runState = 0;
    ui->statusBar->showMessage(tr(" Program terminated"));
}

/*
 * Put the output in the terminal
 */
void MainWindow::updateOutput(){
    output = QString::fromLocal8Bit(processRun.readAllStandardOutput());
    ui->terminal->setPlainText(ui->terminal->toPlainText() + output);
}

/*
 * Put the error in the terminal
 */
void MainWindow::updateError(){
    error = QString::fromLocal8Bit(processRun.readAllStandardError());
    ui->terminal->setPlainText(ui->terminal->toPlainText() + error);
    processRun.terminate();
    runState = 0;
}

/*
 * Send the user input to the processRun
 */
void MainWindow::updateInput(QString data){
    if(runState)
        processRun.write(data.toLocal8Bit());
}

/*
 * About us
 */
void MainWindow::about(){
    QMessageBox::information(this, tr("About"),
                             tr("Project Thermite Version 0.7 \nBy AAAAgito, Lean-Li, MoyuST, PeterLau61, Sylviachelsea, and UndertakerW @CUHKSZ "), QMessageBox::Ok);
}

/*
 * Readme document
 */
void MainWindow::readme(){
    QString readmeCmd = QString(tr("start readme.txt"));
    qDebug() << readmeCmd << endl;
    processReadme.start("cmd", QStringList() << "/c" << readmeCmd);
    processReadme.waitForStarted();
    processReadme.waitForFinished();
}
