#include <QMessageBox>
#include "state.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "startwindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stateLabel->setVisible(false);
    ui->stateButton->setVisible(false);
    ui->nextStepButton->setVisible(false);
    ui->preStepButton->setVisible(false);
    ui->stateButton->setEnabled(false);
    start_window = new StartWindow(this);
    dmfb = new DMFB(this);
    update();
    connect(start_window, SIGNAL(SendData(int,int,int,QString)), dmfb, SLOT(ReceiveData(int,int,int,QString)));
    connect(dmfb, SIGNAL(MainUpdate()), this, SLOT(update()));
    connect(dmfb, SIGNAL(Error(int)), this, SLOT(show_error(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionStart_triggered()
{
    start_window->setModal(true);
    start_window->show();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::go_to_next(){
    dmfb->next_state();
    update();
}

void MainWindow::change_state(){
    switch (dmfb->get_state()) {
    case START_STATE:
        ui->stateLabel->setVisible(false);
        ui->stateButton->setVisible(false);
        ui->nextStepButton->setVisible(false);
        ui->preStepButton->setVisible(false);
        ui->stateButton->setEnabled(false);
        break;

    case INPUT_STATE:
        ui->nextStepButton->setVisible(false);
        ui->preStepButton->setVisible(false);
        ui->stateLabel->setVisible(true);
        ui->stateButton->setVisible(true);
        ui->stateLabel->setText(QString("Set Input"));
        ui->stateButton->setText(QString("Next"));
        ui->stateLabel->setGeometry(dmfb->length*(dmfb->col+2), dmfb->length*(dmfb->row+2), 100, 40);
        ui->stateButton->setGeometry(dmfb->length*(dmfb->col+2)+100, dmfb->length*(dmfb->row+2), 120, 40);
        break;

    case OUTPUT_STATE:
        ui->stateButton->setEnabled(false);
        ui->stateLabel->setText(QString("Set Output"));
        break;

    case MAIN_STATE:
        ui->stateLabel->setVisible(false);
        ui->preStepButton->setVisible(true);
        ui->nextStepButton->setVisible(true);
        ui->stateButton->setText(QString("Restart"));
        ui->preStepButton->setGeometry(ui->stateButton->x(), ui->stateButton->y()-100, 120,40);
        ui->nextStepButton->setGeometry(ui->stateButton->x(), ui->stateButton->y()-150, 120,40);
        break;

    default:
        break;
    }
}

void MainWindow::show_error(int kind){
    enum{
        TABLE_SMALL_ERROR = 0,
        TABLE_BIG_ERROR,
        TOO_MANY_INPUT,
        FILE_NOT_FOUND
    };
    switch(kind){
    case TABLE_SMALL_ERROR:
        QMessageBox::warning(this,"Table Scale Error", "The chip is too small");
        dmfb->set_state(new StartState(dmfb));
        update();
        break;
    case TABLE_BIG_ERROR:
        QMessageBox::warning(this,"Table Scale Error", "The chip is too big");
        dmfb->set_state(new StartState(dmfb));
        update();
        break;
    case TOO_MANY_INPUT:
        QMessageBox::warning(this,"Input Number Error", "There are too many inputs");
        dmfb->set_state(new StartState(dmfb));
        update();
        break;
    case FILE_NOT_FOUND:
        QMessageBox::warning(this,"File Not Found", "There are too many inputs");
        dmfb->set_state(new StartState(dmfb));
        update();
        break;

    default:
        break;
    }
    dmfb->row=-1;
    dmfb->col=-1;
    this->update();
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    switch(dmfb->get_state()){
    case INPUT_STATE:
        if(event->button() == Qt::LeftButton)
            set_tunnel(event);
        break;
    case OUTPUT_STATE:
        if(event->button() == Qt::LeftButton)
            set_tunnel(event);
        break;
    default:
        break;
    }
}

void MainWindow::set_tunnel(QMouseEvent* event){
    const int len = dmfb->length;
    const int x = event->x()/len;
    const int y = (event->y()-len)/len;

    if(x<0 || y<0 || x>dmfb->col+1 || y>dmfb->row+1)
        return;
    if(x>0 && y>0 && x<=dmfb->col && y<=dmfb->row)
        return;

    if(x==0 && y==0)
        return;
    if(x==dmfb->col+1 && y==0)
        return;
    if(x==0 && y==dmfb->row+1)
        return;
    if(x==dmfb->col+1 && y==dmfb->row+1)
        return;

    if(dmfb->get_state() == INPUT_STATE){
        if(dmfb->table[x][y].type == dmfb->NOTHING){
            if(dmfb->in_num==0)
                return;
            dmfb->table[x][y].type = dmfb->INPUT;
            dmfb->in_num--;
            if(dmfb->in_num==0)
                ui->stateButton->setEnabled(true);
        }
        else if(dmfb->table[x][y].type == dmfb->INPUT){
            ui->stateButton->setEnabled(false);
            dmfb->table[x][y].type = dmfb->NOTHING;
            dmfb->in_num++;
        }
    }
    else if(dmfb->get_state() == OUTPUT_STATE){
        if(dmfb->table[x][y].type == dmfb->NOTHING){
            if(dmfb->out_num==0)
                return;
            dmfb->table[x][y].type = dmfb->OUTPUT;
            dmfb->out_num = 0;
            ui->stateButton->setEnabled(true);
        }
        else if(dmfb->table[x][y].type == dmfb->OUTPUT){
            ui->stateButton->setEnabled(false);
            dmfb->table[x][y].type = dmfb->NOTHING;
            dmfb->out_num = 1;
        }
    }
    update();
}


void MainWindow::paintEvent(QPaintEvent*){
    change_state();
    QPainter painter(this);
    const int len = dmfb->length;

    QPen pen;
    QBrush brush;
    QRect rec;

    QPen DefaultPen(Qt::black);

    QBrush Nothing(Qt::white);
    QBrush Input(Qt::red);
    QBrush Output(Qt::green);
    QBrush WashInput(Qt::blue);

    brush = Nothing;
    pen = DefaultPen;
    painter.setPen(pen);
    painter.setBrush(brush);
    for(int i=1; i<=dmfb->row; i++)
        for(int j=1; j<=dmfb->col; j++){
            switch (dmfb->table[j][i].type) {
            default:
                brush = Nothing;
                break;
            }
            painter.drawRect(50+(j-1)*len, 100+(i-1)*len, len, len);
        }
    for(int i=1; i<=dmfb->row; i++){
        switch (dmfb->table[0][i].type) {
        case 1://INPUT
            brush = Input;
            break;
        case 2://OUTPUT
            brush = Output;
            break;
        default:
            pen = DefaultPen;
            brush = Nothing;
            break;
        }
        painter.setPen(pen);
        painter.setBrush(brush);
        rec.setRect(0, 100+(i-1)*len,len,len);
        painter.fillRect(rec,brush);
        painter.drawRect(rec);

        switch (dmfb->table[dmfb->col+1][i].type) {
        case 1://INPUT
            brush = Input;
            break;
        case 2://OUTPUT
            brush = Output;
            break;
        default:
            pen = DefaultPen;
            brush = Nothing;
            break;
        }
        painter.setPen(pen);
        painter.setBrush(brush);
        rec.setRect(50+dmfb->col*len, 100+(i-1)*len, len, len);
        painter.fillRect(rec,brush);
        painter.drawRect(rec);
    }

    for(int i=1; i<=dmfb->col; i++){
        switch (dmfb->table[i][0].type) {
        case 1://INPUT
            brush = Input;
            break;
        case 2://OUTPUT
            brush = Output;
            break;
        default:
            pen = DefaultPen;
            brush = Nothing;
            break;
        }
        painter.setPen(pen);
        painter.setBrush(brush);
        rec.setRect(50+(i-1)*len, 50,len,len);
        painter.fillRect(rec,brush);
        painter.drawRect(rec);

        switch (dmfb->table[i][dmfb->row+1].type) {
        case 1://INPUT
            brush = Input;
            break;
        case 2://OUTPUT
            brush = Output;
            break;
        default:
            pen = DefaultPen;
            brush = Nothing;
            break;
        }
        painter.setPen(pen);
        painter.setBrush(brush);
        rec.setRect(50+(i-1)*len, 100+dmfb->row*len, len, len);
        painter.fillRect(rec,brush);
        painter.drawRect(rec);
    }
}

void MainWindow::on_stateButton_clicked()
{
    switch (dmfb->get_state()) {
    case START_STATE:
        break;
    case INPUT_STATE:
        go_to_next();
        break;
    case OUTPUT_STATE:
        go_to_next();
        break;
    case MAIN_STATE:
        on_actionStart_triggered();
        break;
    default:
        break;
    }
}
