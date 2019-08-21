#include <QString>
#include <QWidget>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    new_window = new NewWindow(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionshow_triggered()
{
    new_window->set_text(QString("show()"));
    new_window->show();
}



void MainWindow::on_actionexec_triggered()
{
    new_window->set_text(QString("exec()"));
    new_window->exec();
}

void MainWindow::on_actionmodal_true_triggered()
{
    new_window->set_text(QString("modal(true)"));
    new_window->setModal(true);
    new_window->show();
}

void MainWindow::on_actionmodal_false_triggered()
{

    new_window->set_text(QString("modal(false)"));
    new_window->setModal(false);
    new_window->show();
}
