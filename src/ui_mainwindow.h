/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionStart;
    QAction *actionExit;
    QWidget *centralWidget;
    QPushButton *nextStepButton;
    QPushButton *preStepButton;
    QLabel *stateLabel;
    QPushButton *stateButton;
    QLCDNumber *lcdNumber;
    QPushButton *AutoButton;
    QPushButton *RestartButton;
    QPushButton *WashButton;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(718, 596);
        actionStart = new QAction(MainWindow);
        actionStart->setObjectName(QString::fromUtf8("actionStart"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        nextStepButton = new QPushButton(centralWidget);
        nextStepButton->setObjectName(QString::fromUtf8("nextStepButton"));
        nextStepButton->setGeometry(QRect(30, 48, 101, 38));
        preStepButton = new QPushButton(centralWidget);
        preStepButton->setObjectName(QString::fromUtf8("preStepButton"));
        preStepButton->setGeometry(QRect(30, 94, 86, 38));
        stateLabel = new QLabel(centralWidget);
        stateLabel->setObjectName(QString::fromUtf8("stateLabel"));
        stateLabel->setGeometry(QRect(50, 300, 45, 21));
        stateButton = new QPushButton(centralWidget);
        stateButton->setObjectName(QString::fromUtf8("stateButton"));
        stateButton->setGeometry(QRect(30, 330, 80, 38));
        lcdNumber = new QLCDNumber(centralWidget);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setGeometry(QRect(30, 10, 64, 23));
        AutoButton = new QPushButton(centralWidget);
        AutoButton->setObjectName(QString::fromUtf8("AutoButton"));
        AutoButton->setGeometry(QRect(30, 140, 86, 38));
        RestartButton = new QPushButton(centralWidget);
        RestartButton->setObjectName(QString::fromUtf8("RestartButton"));
        RestartButton->setGeometry(QRect(30, 190, 86, 38));
        WashButton = new QPushButton(centralWidget);
        WashButton->setObjectName(QString::fromUtf8("WashButton"));
        WashButton->setGeometry(QRect(30, 250, 121, 38));
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 718, 33));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionStart);
        menuFile->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "DMFB", nullptr));
        actionStart->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
#if QT_CONFIG(shortcut)
        actionStart->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+X", nullptr));
#endif // QT_CONFIG(shortcut)
        nextStepButton->setText(QCoreApplication::translate("MainWindow", "Next Step", nullptr));
        preStepButton->setText(QCoreApplication::translate("MainWindow", "Pre Step", nullptr));
        stateLabel->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        stateButton->setText(QCoreApplication::translate("MainWindow", "Next", nullptr));
        AutoButton->setText(QCoreApplication::translate("MainWindow", "Auto:OFF", nullptr));
        RestartButton->setText(QCoreApplication::translate("MainWindow", "ReStart", nullptr));
        WashButton->setText(QCoreApplication::translate("MainWindow", "Wash:OFF", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
