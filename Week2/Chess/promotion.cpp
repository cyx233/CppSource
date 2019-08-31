#include "promotion.h"
#include "ui_promotion.h"
#include <QCoreApplication>
#include <QPixmap>

Promotion::Promotion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Promotion)
{
    ui->setupUi(this);
    this->setModal(true);
    this->setPalette(QPalette(Qt::black,Qt::white));
    this->setModal(true);
    type = 1;
}

Promotion::~Promotion()
{
    delete ui;
}


void Promotion::set_color(int color, int inx, int iny){
    x=inx;y=iny;
    QString dir = QCoreApplication::applicationDirPath() + "/icon/";
    QString file;
    QPixmap pix;
    if(color==1){
        file = dir + "black_queen.png";
        pix = QPixmap(file);
        ui->Queen->setPixmap(pix);

        file = dir + "black_rook.png";
        pix = QPixmap(file);
        ui->Rook->setPixmap(pix);

        file = dir + "black_bishop.png";
        pix = QPixmap(file);
        ui->Bishop->setPixmap(pix);

        file = dir + "black_knight.png";
        pix = QPixmap(file);
        ui->Knight->setPixmap(pix);
    }
    else{
        file = dir + "white_queen.png";
        pix = QPixmap(file);
        ui->Queen->setPixmap(pix);

        file = dir + "white_rook.png";
        pix = QPixmap(file);
        ui->Rook->setPixmap(pix);

        file = dir + "white_bishop.png";
        pix = QPixmap(file);
        ui->Bishop->setPixmap(pix);

        file = dir + "white_knight.png";
        pix = QPixmap(file);
        ui->Knight->setPixmap(pix);
    }
}

void Promotion::on_pushButton_2_clicked()
{
    type = 1;
    close();
}

void Promotion::on_pushButton_clicked()
{
    type = 2;
    close();
}

void Promotion::on_pushButton_3_clicked()
{
    type = 3;
    close();
}


void Promotion::on_pushButton_4_clicked()
{
    type = 4;
    close();
}
