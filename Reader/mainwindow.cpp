#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <iostream>
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    file.clear();
    file = QFileDialog::getOpenFileName(
                            this,
                            "Open Document",
                            QDir::currentPath(),
                            "All Files(*.*)");
    if(!file.isNull())
        read_file();
}

void MainWindow::read_file(){
    QFile f(file);
    if(f.open(QFile::ReadOnly | QFile::Text)){
        QTextStream in(&f);
        ui->textBrowser->setText(in.readAll());
    }
}

void MainWindow::find_pattern(QString pattern){

}

void MainWindow::on_actionClose_triggered()
{
    close();
};
