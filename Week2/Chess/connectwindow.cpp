#include <QMessageBox>
#include "connectwindow.h"
#include "ui_connectwindow.h"

ConnectWindow::ConnectWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectWindow)
{
    ui->setupUi(this);
}

ConnectWindow::~ConnectWindow()
{
    delete ui;
}

void ConnectWindow::on_pushButton_clicked()
{
    int ip1 = ui->ip_1->text().toInt();
    int ip2 = ui->ip_2->text().toInt();
    int ip3 = ui->ip_3->text().toInt();
    int ip4 = ui->ip_4->text().toInt();
    if(ip1<0 || ip2<0 || ip3<0 || ip4<0 ||
            ip1>255 || ip2>255 || ip3>255 || ip4>255){
        QMessageBox::information(this,"Wrong","Invalid IP");
        return;
    }
    QString ip = QString::number(ip1)+"."+QString::number(ip2)+"."+
                 QString::number(ip3)+"."+QString::number(ip4);
    host_ip = QHostAddress(ip);
    port = ui->Port->text().toInt();
    emit StartConnect();
    close();
}

void ConnectWindow::on_pushButton_2_clicked()
{
    close();
}
