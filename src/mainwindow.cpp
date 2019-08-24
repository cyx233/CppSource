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
    timer = new QTimer(this);
    timer->stop();
    update();
    connect(start_window, SIGNAL(SendData(int,int,int,QString)), dmfb, SLOT(ReceiveData(int,int,int,QString)));
    connect(dmfb, SIGNAL(MainUpdate()), this, SLOT(update()));
    connect(dmfb, SIGNAL(Error(int)), this, SLOT(show_error(int)));
    connect(dmfb, SIGNAL(NextLock(int)), this, SLOT(next_lock(int)));
    connect(timer, SIGNAL(timeout()), this, SLOT(on_nextStepButton_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//////////////////////////////////////////////////////
/// MenuBar
///
void MainWindow::on_actionStart_triggered()
{
    timer->stop();
    ui->AutoButton->setText(QString("Auto:OFF"));
    ui->WashButton->setText(QString("Wash:OFF"));
    start_window->setModal(true);
    start_window->show();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

//////////////////////////////////////////////////////


//////////////////////////////////////////////////////
/// Button
///
void MainWindow::on_AutoButton_clicked()
{
    if(timer->isActive()){
        timer->stop();
        ui->AutoButton->setText(QString("Auto:OFF"));
    }
    else {
        timer->start(250);
        ui->AutoButton->setText(QString("Auto:ON"));
    }
}

void MainWindow::on_WashButton_clicked()
{
    if(dmfb->wash_on){
        dmfb->wash_on=0;
        ui->WashButton->setText(QString("Wash:OFF"));
    }
    else {
        dmfb->wash_on=1;
        ui->WashButton->setText(QString("Wash:ON"));
    }
}

void MainWindow::on_nextStepButton_clicked()
{
    if(dmfb->get_time() == (int)dmfb->time_command.size()-1)
        return;
    dmfb->redo();
    dmfb->show();
    if(dmfb->wash_on){
        dmfb->wash();
        dmfb->wash_check();
    }
}

void MainWindow::on_preStepButton_clicked()
{
    if(dmfb->get_time() == 0)
        return;
    dmfb->undo();
    ui->AutoButton->setEnabled(true);
    ui->nextStepButton->setEnabled(true);
    dmfb->show();
}

void MainWindow::on_RestartButton_clicked()
{
    ui->nextStepButton->setEnabled(true);
    ui->AutoButton->setEnabled(true);
    dmfb->begin();

    vector<int>temp1;
    vector<vector<int>>temp2(dmfb->row+2,temp1);
    dmfb->trace_exist.assign(dmfb->col+2,temp2);
    dmfb->show();
}

void MainWindow::on_stateButton_clicked()
{
    switch (dmfb->get_state()) {
    case State::START_STATE:
        break;

    case State::INPUT_STATE:
        go_to_next();
        break;

    case State::OUTPUT_STATE:
        go_to_next();
        dmfb->set_default_wash();
        break;

    case State::WASH_STATE:
        go_to_next();
        dmfb->set_default_waste();
        break;

    case State::WASTE_STATE:
        go_to_next();
        dmfb->copy = dmfb->table;
        dmfb->begin();
        break;

    case State::MAIN_STATE:
        on_actionStart_triggered();
        break;
    default:
        break;
    }
}

void MainWindow::next_lock(int a){
    ui->AutoButton->setEnabled(!a);
    ui->nextStepButton->setEnabled(!a);
    ui->preStepButton->setEnabled(!a);
    ui->RestartButton->setEnabled(!a);
    ui->stateButton->setEnabled(!a);
}
//////////////////////////////////////////////////////


void MainWindow::show_error(int kind){
    switch(kind){
    case DMFB::TABLE_SMALL_ERROR:
        QMessageBox::warning(this,"Table Scale Error", "The chip is too small");
        dmfb->set_state(new StartState(dmfb));
        dmfb->row=-1;
        dmfb->col=-1;
        break;
    case DMFB::TABLE_BIG_ERROR:
        QMessageBox::warning(this,"Table Scale Error", "The chip is too big");
        dmfb->set_state(new StartState(dmfb));
        dmfb->row=-1;
        dmfb->col=-1;
        break;
    case DMFB::TOO_MANY_INPUT:
        QMessageBox::warning(this,"Input Number Error", "There are too many inputs");
        dmfb->set_state(new StartState(dmfb));
        dmfb->row=-1;
        dmfb->col=-1;
        break;
    case DMFB::FILE_NOT_FOUND:
        QMessageBox::warning(this,"File Not Found", "There is not such a file");
        dmfb->set_state(new StartState(dmfb));
        dmfb->row=-1;
        dmfb->col=-1;
        break;
    case DMFB::COMMAND_ERROR:
        QMessageBox::warning(this,"Command Error", "Wrong command in file");
        dmfb->set_state(new StartState(dmfb));
        dmfb->row=-1;
        dmfb->col=-1;
        break;
    case DMFB::OUT_OF_CHIP:
        QMessageBox::warning(this,"Run Error", "Out of the chip");
        dmfb->set_state(new StartState(dmfb));
        dmfb->row=-1;
        dmfb->col=-1;
        break;
    case DMFB::RUN_ERROR:
        QMessageBox::warning(this,"Run Error", "Run Error");
        timer->stop();
        dmfb->wash_on=0;
        ui->AutoButton->setText(QString("Auto:OFF"));
        ui->WashButton->setText(QString("Wash:OFF"));
        ui->nextStepButton->setEnabled(false);
        ui->AutoButton->setEnabled(false);
        dmfb->wash_on=0;
        ui->nextStepButton->setEnabled(false);
        break;
    case DMFB::NO_WASH:
        QMessageBox::warning(this,"NO WASH", "No space for wash input and waste");
        on_stateButton_clicked();
        break;
    case DMFB::WASH_ERROR:
        QMessageBox::warning(this,"WASH ERROR", "Pullution was casued");
        break;
    default:
        break;
    }
    update();
}


//////////////////////////////////////////////////////
/// Mouse
///
void MainWindow::mousePressEvent(QMouseEvent *event){
    switch(dmfb->get_state()){
    case State::INPUT_STATE:
        if(event->button() == Qt::LeftButton)
            set_tunnel(event);
        break;

    case State::OUTPUT_STATE:
        if(event->button() == Qt::LeftButton)
            set_tunnel(event);
        break;

    case State::WASH_STATE:
        if(event->button() == Qt::LeftButton)
            set_tunnel(event);
        break;

    case State::WASTE_STATE:
        if(event->button() == Qt::LeftButton)
            set_tunnel(event);
        break;

    case State::MAIN_STATE:
        if(event->button() == Qt::RightButton)
            set_barrier(event);
        break;
    default:
        break;
    }
}

void MainWindow::set_tunnel(QMouseEvent* event){
    const int len = dmfb->length;
    const int x = (event->x()-150)/len;
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

    switch (dmfb->get_state()) {
    case State::INPUT_STATE:
        if(dmfb->table[x][y].type == DMFB::NOTHING){
            if(dmfb->in_num==0)
                return;
            dmfb->table[x][y].type = DMFB::INPUT;
            dmfb->in_num--;
            if(dmfb->in_num==0)
                ui->stateButton->setEnabled(true);
        }
        else if(dmfb->table[x][y].type == DMFB::INPUT){
            ui->stateButton->setEnabled(false);
            dmfb->table[x][y].type = DMFB::NOTHING;
            dmfb->in_num++;
        }
        break;

    case State::OUTPUT_STATE:
        if(dmfb->table[x][y].type == DMFB::NOTHING){
            if(dmfb->out_num==0)
                return;
            dmfb->table[x][y].type = DMFB::OUTPUT;
            dmfb->out_num = 0;
            ui->stateButton->setEnabled(true);
        }
        else if(dmfb->table[x][y].type == DMFB::OUTPUT){
            ui->stateButton->setEnabled(false);
            dmfb->table[x][y].type = DMFB::NOTHING;
            dmfb->out_num = 1;
        }
        break;

    case State::WASH_STATE:
        if(dmfb->table[x][y].type == DMFB::NOTHING){
            if(dmfb->wash_num==0)
                return;
            dmfb->table[x][y].type = DMFB::WASH_PUT;
            dmfb->wash_num = 0;
        }
        else if(dmfb->table[x][y].type == DMFB::WASH_PUT){
            dmfb->table[x][y].type = DMFB::NOTHING;
            dmfb->wash_num = 1;
        }
        break;

    case State::WASTE_STATE:
        if(dmfb->table[x][y].type == DMFB::NOTHING){
            if(dmfb->waste_num==0)
                return;
            dmfb->table[x][y].type = DMFB::WASTE;
            dmfb->waste_num = 0;
        }
        else if(dmfb->table[x][y].type == DMFB::WASTE){
            dmfb->table[x][y].type = DMFB::NOTHING;
            dmfb->waste_num = 1;
        }
        break;

    default:
        break;
    }

    update();
}

void MainWindow::set_barrier(QMouseEvent* event){
    const int len = dmfb->length;
    const int x = (event->x()-150)/len;
    const int y = (event->y()-len)/len;

    if(x<1 || y<1 || x>dmfb->col || y>dmfb->row)
        return;

    if(!ui->nextStepButton->isEnabled())
        return;

    if(dmfb->table[x][y].is_barrier == 0){
        dmfb->table[x][y].is_barrier = 1;
        dmfb->copy[x][y].is_barrier = 1;
    }
    else if(dmfb->table[x][y].is_barrier == 1){
        dmfb->table[x][y].is_barrier = 0;
        dmfb->copy[x][y].is_barrier = 0;
    }
    update();
}
//////////////////////////////////////////////////////


//////////////////////////////////////////////////////
///	State
///
void MainWindow::paintEvent(QPaintEvent*){
    change_state();
    QPainter painter(this);
    const int len = dmfb->length;

    ui->lcdNumber->display(dmfb->get_time());

    QPen pen;
    QBrush brush;
    QRect rec;

    QPen DefaultPen(Qt::black);

    QBrush Nothing(Qt::white);
    QBrush Input(Qt::red);
    QBrush Output(Qt::green);
    QBrush Waste(Qt::yellow);
    QBrush WashInput(Qt::blue);
    QBrush Barrier(Qt::gray);

    for(int i=1; i<=dmfb->row; i++)
        for(int j=1; j<=dmfb->col; j++){
            brush = Nothing;
            pen = DefaultPen;
            painter.setPen(pen);
            painter.setBrush(brush);
            rec.setRect(200+(j-1)*len, 100+(i-1)*len, len, len);
            painter.drawRect(rec);
            if(dmfb->table[j][i].trace.size()){
                for(int k=0; k<(int)dmfb->table[j][i].trace.size(); k++){
                    if(dmfb->trace_exist[j][i][k]){
                        int id = dmfb->table[j][i].trace[k];
                        QColor temp = dmfb->colors[id];
                        temp.setAlpha(60);
                        brush.setColor(temp);
                        painter.setBrush(brush);
                        painter.drawRect(rec);
                    }
                }
            }
        }

    brush = Barrier;
    pen = DefaultPen;
    painter.setPen(pen);
    painter.setBrush(brush);
    for(int i=1; i<=dmfb->row; i++)
        for(int j=1; j<=dmfb->col; j++){
            if(dmfb->table[j][i].is_barrier){
                rec.setRect(200+(j-1)*len, 100+(i-1)*len, len, len);
                painter.drawRect(rec);
            }
        }

    brush = Nothing;
    pen = DefaultPen;
    painter.setPen(pen);
    painter.setBrush(brush);
    for(int i=1; i<=dmfb->row; i++)
        for(int j=1; j<=dmfb->col; j++){
            rec.setRect(200+(j-1)*len, 100+(i-1)*len, len, len);
            painter.setBrush(brush);
            painter.setPen(pen);
            int r = (int)dmfb->table[j][i].size;
            int id = dmfb->table[j][i].id;
            QColor trace;
            switch (dmfb->table[j][i].type) {
            case DMFB::NORMAL:
                rec.setX(int(rec.x()+(double)(len-2*r)/2));
                rec.setY(int(rec.y()+(double)(len-2*r)/2));
                rec.setWidth(2*r);
                rec.setHeight(2*r);
                brush.setColor(dmfb->colors[id]);
                painter.setBrush(brush);
                painter.drawEllipse(rec);
                break;
            case DMFB::INPUT_DROP:
                rec.setX(int(rec.x()+(double)(len-2*r)/2));
                rec.setY(int(rec.y()+(double)(len-2*r)/2));
                rec.setWidth(2*r);
                rec.setHeight(2*r);
                brush.setColor(dmfb->colors[id]);
                painter.setBrush(brush);
                painter.drawEllipse(rec);
                break;
            case DMFB::MERGE_H:
                rec.setX(int(rec.x()-len+(double)3*(len-2*r)/2));
                rec.setY((len-2*r)/2+rec.y());
                rec.setWidth(3*2*r);
                rec.setHeight(2*r);
                brush.setColor(dmfb->colors[id]);
                painter.setBrush(brush);
                painter.drawEllipse(rec);
                break;
            case DMFB::MERGE_V:
                rec.setX((len-2*r)/2+rec.x());
                rec.setY(int(rec.y()-len+(double)3*(len-2*r)/2));
                rec.setWidth(2*r);
                rec.setHeight(3*2*r);
                brush.setColor(dmfb->colors[id]);
                painter.setBrush(brush);
                painter.drawEllipse(rec);
                break;
            case DMFB::WASH_DROP:
                rec.setX(int(rec.x()+(double)(len-2*r)/2));
                rec.setY(int(rec.y()+(double)(len-2*r)/2));
                rec.setWidth(2*r);
                rec.setHeight(2*r);
                brush = WashInput;
                painter.setBrush(brush);
                painter.drawEllipse(rec);
                break;
            default:
                break;
            }
            brush = Nothing;
            pen = DefaultPen;
        }
    for(int i=1; i<=dmfb->row; i++){
        switch (dmfb->table[0][i].type) {
        case DMFB::INPUT:
            brush = Input;
            break;
        case DMFB::OUTPUT:
            brush = Output;
            break;
        case DMFB::WASH_PUT:
            brush = WashInput;
            break;
        case DMFB::WASTE:
            brush = Waste;
            break;
        default:
            pen = DefaultPen;
            brush = Nothing;
            break;
        }
        painter.setPen(pen);
        painter.setBrush(brush);
        rec.setRect(150, 100+(i-1)*len,len,len);
        painter.fillRect(rec,brush);
        painter.drawRect(rec);

        switch (dmfb->table[dmfb->col+1][i].type) {
        case DMFB::INPUT:
            brush = Input;
            break;
        case DMFB::OUTPUT:
            brush = Output;
            break;
        case DMFB::WASH_PUT:
            brush = WashInput;
            break;
        case DMFB::WASTE:
            brush = Waste;
            break;
        default:
            pen = DefaultPen;
            brush = Nothing;
            break;
        }
        painter.setPen(pen);
        painter.setBrush(brush);
        rec.setRect(200+dmfb->col*len, 100+(i-1)*len, len, len);
        painter.fillRect(rec,brush);
        painter.drawRect(rec);
    }

    for(int i=1; i<=dmfb->col; i++){
        switch (dmfb->table[i][0].type) {
        case DMFB::INPUT:
            brush = Input;
            break;
        case DMFB::OUTPUT:
            brush = Output;
            break;
        case DMFB::WASH_PUT:
            brush = WashInput;
            break;
        case DMFB::WASTE:
            brush = Waste;
            break;
        default:
            pen = DefaultPen;
            brush = Nothing;
            break;
        }
        painter.setPen(pen);
        painter.setBrush(brush);
        rec.setRect(200+(i-1)*len, 50,len,len);
        painter.fillRect(rec,brush);
        painter.drawRect(rec);

        switch (dmfb->table[i][dmfb->row+1].type) {
        case DMFB::INPUT:
            brush = Input;
            break;
        case DMFB::OUTPUT:
            brush = Output;
            break;
        case DMFB::WASH_PUT:
            brush = WashInput;
            break;
        case DMFB::WASTE:
            brush = Waste;
            break;
        default:
            pen = DefaultPen;
            brush = Nothing;
            break;
        }
        painter.setPen(pen);
        painter.setBrush(brush);
        rec.setRect(200+(i-1)*len, 100+dmfb->row*len, len, len);
        painter.fillRect(rec,brush);
        painter.drawRect(rec);
    }

    if(dmfb->get_time() == (int)dmfb->time_command.size()-1){
        for(int i=1; i<=dmfb->row; i++)
            for(int j=1; j<=dmfb->col; j++){
                rec.setRect(200+(j-1)*len+len/3, 100+(i-1)*len+len/3, len, len);
                int result=0;
                vector<bool> a(dmfb->id_count[DMFB::NORMAL]+1,0);
                for(int k=0; k<(int)dmfb->table[j][i].trace.size(); k++){
                    int id = dmfb->table[j][i].trace[k];
                    a[id]=1;
                }
                for(int k=0; k<(int)a.size(); k++)
                    if(a[k])
                        result++;
                painter.drawText(rec,QString::number(result));
            }
    }
}

void MainWindow::go_to_next(){
    dmfb->next_state();
    update();
}

void MainWindow::change_state(){
    switch (dmfb->get_state()) {
    case State::START_STATE:
        ui->stateLabel->setVisible(false);
        ui->stateButton->setVisible(false);
        ui->nextStepButton->setVisible(false);
        ui->preStepButton->setVisible(false);
        ui->stateButton->setEnabled(false);
        ui->AutoButton->setVisible(false);
        ui->lcdNumber->setVisible(false);
        ui->RestartButton->setVisible(false);
        ui->WashButton->setVisible(false);
        timer->stop();
        dmfb->wash_on=0;
        break;

    case State::INPUT_STATE:
        ui->nextStepButton->setVisible(false);
        ui->preStepButton->setVisible(false);
        ui->AutoButton->setVisible(false);
        ui->lcdNumber->setVisible(false);
        ui->RestartButton->setVisible(false);
        ui->WashButton->setVisible(false);

        ui->stateLabel->setVisible(true);
        ui->stateButton->setVisible(true);
        ui->AutoButton->setEnabled(true);
        ui->nextStepButton->setEnabled(true);

        if(dmfb->in_num != 0)
            ui->stateButton->setEnabled(false);
        ui->stateLabel->setText(QString("Set Input"));
        ui->stateButton->setText(QString("Next"));
        ui->stateLabel->setGeometry(0, 360, 100, 40);
        ui->stateButton->setGeometry(0, 410, 120, 40);

        dmfb->set_time(0);

        break;

    case State::OUTPUT_STATE:
        if(dmfb->out_num == 1)
            ui->stateButton->setEnabled(false);
        ui->stateLabel->setText(QString("Set Output"));
        break;

    case State::WASH_STATE:
        ui->stateLabel->setText(QString("Set Wash"));
        break;

    case State::WASTE_STATE:
        ui->stateLabel->setText(QString("Set Waste"));
        break;

    case State::MAIN_STATE:
        ui->stateLabel->setVisible(false);

        ui->lcdNumber->setVisible(true);
        ui->preStepButton->setVisible(true);
        ui->AutoButton->setVisible(true);
        ui->nextStepButton->setVisible(true);
        ui->RestartButton->setVisible(true);
        ui->stateButton->setText(QString("New Start"));
        if(!dmfb->wash_num && !dmfb->waste_num)
            ui->WashButton->setVisible(true);
        else {
            ui->WashButton->setVisible(false);
        }

        ui->lcdNumber->setGeometry(0, 60, 120, 40);
        ui->AutoButton->setGeometry(0, 110, 120, 40);
        ui->nextStepButton->setGeometry(0, 160, 120, 40);
        ui->preStepButton->setGeometry(0, 210, 120, 40);
        ui->RestartButton->setGeometry(0, 260, 120, 40);
        ui->WashButton->setGeometry(0,310,120,40);
        break;

    default:
        break;
    }
}

//////////////////////////////////////////////////////
