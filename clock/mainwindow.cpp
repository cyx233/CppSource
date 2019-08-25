#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on = 0;
    time_now=0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this,SLOT(time_go()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPen pen;
    QBrush brush;
    QRect rec;

    pen.setStyle(Qt::SolidLine);
    brush.setStyle(Qt::SolidPattern);

    pen.setColor(Qt::black);
    brush.setColor(Qt::white);

    painter.setPen(pen);
    painter.setBrush(brush);

    int d;
    if(width()>height()){
        d = height()-40;
    }
    else{
        d = width()-40;
    }

    painter.translate((width()-d)/2, (height()-d)/2);
    rec.setRect(0,0,d,d);
    painter.drawEllipse(rec);


    painter.translate(d/2,d/2);
    painter.save();
    for(int i=1;i<=60;i++){
        const int angle = 6;
        painter.rotate(angle);
        if(i%5 == 0){
            pen.setWidth(10);
            painter.setPen(pen);
            painter.drawLine(0,-d/2,0,-d/2*4/5);
            painter.drawText(0,-d/2*4/5+20,QString::number(i));
        }
        else{
            pen.setWidth(5);
            painter.setPen(pen);
            painter.drawLine(0,-d/2,0,-d/2*6/7);
        }
    }

    painter.restore();

    QPolygon poly;
    pen.setWidth(1);
    brush.setColor(Qt::red);
    painter.setPen(pen);
    painter.setBrush(brush);

    int angle = time_now*6;
    painter.rotate(angle);
    poly.setPoints(4,d/40,0,0,-d/2,-d/20,0,0,d/40);
    painter.drawPolygon(poly);
}

void MainWindow::time_go(){
    if(time_now<60)
        time_now++;
    else {
        time_now=1;
    }
    ui->lcdNumber->display(time_now);
    update();
}
void MainWindow::on_pushButton_clicked()
{
    //start
    timer->start(1000);
    ui->pushButton_3->setEnabled(false);
}

void MainWindow::on_pushButton_2_clicked()
{
    //pause
    timer->stop();
    ui->pushButton_3->setEnabled(true);
}

void MainWindow::on_pushButton_3_clicked()
{
    //restart
    time_now = 0;
    update();
}
