#include "createserver.h"
#include "ui_createserver.h"
#include <QMessageBox>

CreateServer::CreateServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateServer)
{
    ui->setupUi(this);
    this->setModal(true);
}

CreateServer::~CreateServer()
{
    delete ui;
}

void CreateServer::on_pushButton_clicked()
{
    bool ok=1;
    int flag=1;
    int ip1 = ui->ip_1->text().toInt(&ok);
    if(!ok)
       flag=0;
    int ip2 = ui->ip_2->text().toInt(&ok);
    if(!ok)
       flag=0;
    int ip3 = ui->ip_3->text().toInt(&ok);
    if(!ok)
       flag=0;
    int ip4 = ui->ip_4->text().toInt(&ok);
    if(!ok)
       flag=0;
    if(ip1<0 || ip2<0 || ip3<0 || ip4<0 ||
            ip1>255 || ip2>255 || ip3>255 || ip4>255 || flag==0){
        QMessageBox::information(this,"Wrong","Invalid IP");
        return;
    }
    QString ip = QString::number(ip1)+"."+QString::number(ip2)+"."+
                 QString::number(ip3)+"."+QString::number(ip4);
    host_ip = QHostAddress(ip);
    port = ui->Port->text().toInt();
    emit FinishCreate();
    close();
}

void CreateServer::on_pushButton_2_clicked()
{
    close();
}

void CreateServer::set_ip(QString ip){
    ui->ip_label->setText(ip);
}

void CreateServer::on_pushButton_3_clicked()
{
    emit StopConnect();
    close();
}
