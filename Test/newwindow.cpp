#include <QString>
#include "newwindow.h"
#include "ui_newwindow.h"

NewWindow::NewWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewWindow)
{
    ui->setupUi(this);
}

NewWindow::~NewWindow()
{
    delete ui;
}

void NewWindow::set_text(QString s){
    ui->label->setText(s);
}
