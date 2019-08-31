#ifndef CREATESERVER_H
#define CREATESERVER_H

#include <QDialog>
#include <QHostAddress>

namespace Ui {
class CreateServer;
}

class CreateServer : public QDialog
{
    Q_OBJECT

public:
    explicit CreateServer(QWidget *parent = nullptr);
    ~CreateServer();
    QHostAddress get_ip(){return host_ip;}
    int get_port(){return port;}
    void set_ip(QString ip);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

signals:
    void FinishCreate();
    void StopConnect();

private:
    Ui::CreateServer *ui;
    QHostAddress host_ip;
    int port;
};


#endif // CREATSERVER_H
