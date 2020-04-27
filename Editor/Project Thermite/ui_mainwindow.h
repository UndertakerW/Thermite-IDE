/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <codeeditor.h>
#include <terminal.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNewFile;
    QAction *actionOpen;
    QAction *actionSelectAll;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionAbout;
    QAction *actionReadme;
    QAction *actionSaveFile;
    QAction *actionSetting;
    QAction *actionCompile;
    QAction *actionAssemble;
    QAction *actionSimulate;
    QAction *actionRun;
    QAction *actionSaveAs;
    QAction *actionCopy;
    QAction *actionCut;
    QAction *actionPaste;
    QWidget *centralWidget;
    CodeEditor *editor;
    Terminal *terminal;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menuEdit;
    QMenu *menuHelp;
    QMenu *menuRun;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1280, 720);
        QFont font;
        font.setFamily(QString::fromUtf8("Myriad Pro"));
        font.setPointSize(12);
        font.setBold(false);
        font.setWeight(50);
        MainWindow->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ICONS/Icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        actionNewFile = new QAction(MainWindow);
        actionNewFile->setObjectName(QString::fromUtf8("actionNewFile"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/ICONS/New.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNewFile->setIcon(icon1);
        QFont font1;
        font1.setFamily(QString::fromUtf8("Myriad Pro"));
        font1.setPointSize(12);
        actionNewFile->setFont(font1);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/ICONS/Open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon2);
        actionOpen->setFont(font1);
        actionSelectAll = new QAction(MainWindow);
        actionSelectAll->setObjectName(QString::fromUtf8("actionSelectAll"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/ICONS/SelectAll.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelectAll->setIcon(icon3);
        actionSelectAll->setFont(font1);
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/ICONS/Undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndo->setIcon(icon4);
        actionUndo->setFont(font1);
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/ICONS/Redo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRedo->setIcon(icon5);
        actionRedo->setFont(font1);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/ICONS/About.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAbout->setIcon(icon6);
        actionAbout->setFont(font1);
        actionReadme = new QAction(MainWindow);
        actionReadme->setObjectName(QString::fromUtf8("actionReadme"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/ICONS/Document.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReadme->setIcon(icon7);
        actionReadme->setFont(font1);
        actionSaveFile = new QAction(MainWindow);
        actionSaveFile->setObjectName(QString::fromUtf8("actionSaveFile"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/ICONS/Save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveFile->setIcon(icon8);
        actionSaveFile->setFont(font1);
        actionSetting = new QAction(MainWindow);
        actionSetting->setObjectName(QString::fromUtf8("actionSetting"));
        actionSetting->setEnabled(true);
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/ICONS/Settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSetting->setIcon(icon9);
        actionSetting->setFont(font1);
        actionSetting->setVisible(true);
        actionCompile = new QAction(MainWindow);
        actionCompile->setObjectName(QString::fromUtf8("actionCompile"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/ICONS/Compile.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCompile->setIcon(icon10);
        actionCompile->setFont(font1);
        actionAssemble = new QAction(MainWindow);
        actionAssemble->setObjectName(QString::fromUtf8("actionAssemble"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/ICONS/Assemble.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAssemble->setIcon(icon11);
        actionAssemble->setFont(font1);
        actionSimulate = new QAction(MainWindow);
        actionSimulate->setObjectName(QString::fromUtf8("actionSimulate"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/ICONS/Simulate.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSimulate->setIcon(icon12);
        actionSimulate->setFont(font1);
        actionRun = new QAction(MainWindow);
        actionRun->setObjectName(QString::fromUtf8("actionRun"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/ICONS/Run.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRun->setIcon(icon13);
        actionRun->setFont(font1);
        actionSaveAs = new QAction(MainWindow);
        actionSaveAs->setObjectName(QString::fromUtf8("actionSaveAs"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/ICONS/SaveAs.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSaveAs->setIcon(icon14);
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/ICONS/Copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy->setIcon(icon15);
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/ICONS/Cut.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCut->setIcon(icon16);
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/ICONS/Paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste->setIcon(icon17);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        editor = new CodeEditor(centralWidget);
        editor->setObjectName(QString::fromUtf8("editor"));
        editor->setGeometry(QRect(10, 0, 1251, 561));
        terminal = new Terminal(centralWidget);
        terminal->setObjectName(QString::fromUtf8("terminal"));
        terminal->setGeometry(QRect(10, 570, 1251, 81));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1280, 19));
        menuBar->setFont(font);
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu->setFont(font1);
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuEdit->setFont(font1);
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuHelp->setFont(font1);
        menuRun = new QMenu(menuBar);
        menuRun->setObjectName(QString::fromUtf8("menuRun"));
        menuRun->setFont(font1);
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(statusBar->sizePolicy().hasHeightForWidth());
        statusBar->setSizePolicy(sizePolicy);
        statusBar->setFont(font);
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuRun->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menu->addAction(actionNewFile);
        menu->addAction(actionOpen);
        menu->addAction(actionSaveFile);
        menu->addAction(actionSaveAs);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionPaste);
        menuEdit->addAction(actionSelectAll);
        menuHelp->addAction(actionSetting);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionReadme);
        menuRun->addAction(actionCompile);
        menuRun->addAction(actionAssemble);
        menuRun->addAction(actionSimulate);
        menuRun->addAction(actionRun);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Project Thermite", nullptr));
        actionNewFile->setText(QApplication::translate("MainWindow", "New", nullptr));
#ifndef QT_NO_TOOLTIP
        actionNewFile->setToolTip(QApplication::translate("MainWindow", "Create a C++ code file", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionNewFile->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_NO_SHORTCUT
        actionOpen->setText(QApplication::translate("MainWindow", "Open", nullptr));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("MainWindow", "Open a C++ code file", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionSelectAll->setText(QApplication::translate("MainWindow", "Select All", nullptr));
#ifndef QT_NO_TOOLTIP
        actionSelectAll->setToolTip(QApplication::translate("MainWindow", "Select all text", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionSelectAll->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", nullptr));
#endif // QT_NO_SHORTCUT
        actionUndo->setText(QApplication::translate("MainWindow", "Undo", nullptr));
#ifndef QT_NO_SHORTCUT
        actionUndo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", nullptr));
#endif // QT_NO_SHORTCUT
        actionRedo->setText(QApplication::translate("MainWindow", "Redo", nullptr));
#ifndef QT_NO_SHORTCUT
        actionRedo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+Z", nullptr));
#endif // QT_NO_SHORTCUT
        actionAbout->setText(QApplication::translate("MainWindow", "About", nullptr));
#ifndef QT_NO_TOOLTIP
        actionAbout->setToolTip(QApplication::translate("MainWindow", "About Project Thermite", nullptr));
#endif // QT_NO_TOOLTIP
        actionReadme->setText(QApplication::translate("MainWindow", "Readme", nullptr));
#ifndef QT_NO_TOOLTIP
        actionReadme->setToolTip(QApplication::translate("MainWindow", "View readme", nullptr));
#endif // QT_NO_TOOLTIP
        actionSaveFile->setText(QApplication::translate("MainWindow", "Save", nullptr));
#ifndef QT_NO_TOOLTIP
        actionSaveFile->setToolTip(QApplication::translate("MainWindow", "Save the file", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionSaveFile->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_NO_SHORTCUT
        actionSetting->setText(QApplication::translate("MainWindow", "Settings", nullptr));
#ifndef QT_NO_TOOLTIP
        actionSetting->setToolTip(QApplication::translate("MainWindow", "Change the settings", nullptr));
#endif // QT_NO_TOOLTIP
        actionCompile->setText(QApplication::translate("MainWindow", "Compile", nullptr));
#ifndef QT_NO_TOOLTIP
        actionCompile->setToolTip(QApplication::translate("MainWindow", "Compile (C++ Code)", nullptr));
#endif // QT_NO_TOOLTIP
        actionAssemble->setText(QApplication::translate("MainWindow", "Assemble", nullptr));
#ifndef QT_NO_TOOLTIP
        actionAssemble->setToolTip(QApplication::translate("MainWindow", "Assemble (MIPS Assembly Language)", nullptr));
#endif // QT_NO_TOOLTIP
        actionSimulate->setText(QApplication::translate("MainWindow", "Simulate", nullptr));
#ifndef QT_NO_TOOLTIP
        actionSimulate->setToolTip(QApplication::translate("MainWindow", "Simulate (MIPS Machine Language)", nullptr));
#endif // QT_NO_TOOLTIP
        actionRun->setText(QApplication::translate("MainWindow", "Run", nullptr));
#ifndef QT_NO_TOOLTIP
        actionRun->setToolTip(QApplication::translate("MainWindow", "Run (C++ Code)", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        actionRun->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", nullptr));
#endif // QT_NO_SHORTCUT
        actionSaveAs->setText(QApplication::translate("MainWindow", "Save As", nullptr));
        actionCopy->setText(QApplication::translate("MainWindow", "Copy", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCopy->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", nullptr));
#endif // QT_NO_SHORTCUT
        actionCut->setText(QApplication::translate("MainWindow", "Cut", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCut->setShortcut(QApplication::translate("MainWindow", "Ctrl+X", nullptr));
#endif // QT_NO_SHORTCUT
        actionPaste->setText(QApplication::translate("MainWindow", "Paste", nullptr));
#ifndef QT_NO_SHORTCUT
        actionPaste->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", nullptr));
#endif // QT_NO_SHORTCUT
        menu->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", nullptr));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", nullptr));
        menuRun->setTitle(QApplication::translate("MainWindow", "Run", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
