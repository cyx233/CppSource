#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSignalMapper>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSignalMapper* m = new QSignalMapper(this);
    m->setMapping(ui->pushButton_0,0);
    m->setMapping(ui->pushButton_1,1);
    m->setMapping(ui->pushButton_2,2);
    m->setMapping(ui->pushButton_3,3);
    m->setMapping(ui->pushButton_4,4);
    m->setMapping(ui->pushButton_5,5);
    m->setMapping(ui->pushButton_6,6);
    m->setMapping(ui->pushButton_7,7);
    m->setMapping(ui->pushButton_8,8);
    m->setMapping(ui->pushButton_9,9);
    m->setMapping(ui->AddButton,10);
    m->setMapping(ui->MinusButton,11);
    m->setMapping(ui->EqualButton,12);
    m->setMapping(ui->QuitButton,13);

    connect(m, SIGNAL(mapped(int)),this, SLOT(pressEvent()));
}

int MainWindow::calculate(QString text){
}

void MainWindow::pressEvent(int press){
    int tot=0;
    switch (press) {
    case 10:
        break;
    case 11:
        break;
    case 12:
        ans = calculate(ui->textBrowser->toPlainText());
        break;
    case 13:
        close();
        break;
    default:
        break;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
