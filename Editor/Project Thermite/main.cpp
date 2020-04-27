#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication thermite(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return thermite.exec();
}
