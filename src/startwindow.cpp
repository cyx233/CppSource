#include <QFileDialog>
#include "startwindow.h"
#include "ui_startwindow.h"


StartWindow::StartWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::on_pushButton_clicked()
{
   //OK clicked
    emit SendData(ui->RowNum->value(), ui->ColNum->value(), ui->InputNum->value(), file);
    this->close();
}

void StartWindow::on_pushButton_2_clicked()
{
   //Cancel clicked
    this->close();
}

void StartWindow::on_pushButton_3_clicked()
{
    //Open File
    file = QFileDialog::getOpenFileName(
                this,
                "Open Document",
                QDir::currentPath(),
                "All fills(*.*)");
    if(!file.isNull()){
        ui->lineEdit->setText(file);
    }
}
