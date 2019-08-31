#include "colorchoose.h"
#include "ui_colorchoose.h"
#include <QCoreApplication>

ColorChoose::ColorChoose(QWidget *parent) :
    QDialog (parent),
    ui(new Ui::ColorChoose)
{
    ui->setupUi(this);
    color = 0;

    this->setModal(true);

    QPixmap black,white;
    QString dir = QCoreApplication::applicationDirPath();

    QString file = dir + "/icon/black_king.png";
    black = QPixmap(file);

    file = dir + "/icon/white_king.png";
    white = QPixmap(file);

    ui->label->setPixmap(black);
    ui->label_2->setPixmap(white);
    this->setPalette(QPalette(Qt::black,Qt::white));
}

ColorChoose::~ColorChoose()
{
    delete ui;
}

void ColorChoose::on_pushButton_clicked()
{
    color = 1;
    close();
}

void ColorChoose::on_pushButton_2_clicked()
{
    color = 0;
    close();
}
