#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H

#include <QDialog>
#include <QHostAddress>

namespace Ui {
class ConnectWindow;
}

class ConnectWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectWindow(QWidget *parent = nullptr);
    ~ConnectWindow();
    QHostAddress get_ip(){return host_ip;}
    int get_port(){return port;}

signals:
    void StartConnect();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ConnectWindow *ui;
    QHostAddress host_ip;
    int port;
};

#endif // CONNECTWINDOW_H
