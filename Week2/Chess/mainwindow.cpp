#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QRect>
#include <QCoreApplication>
#include <QPalette>
#include <QWidget>
#include <QMessageBox>
#include <QObject>
#include <QFileDialog>
#include <QNetworkInterface>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    set_default_table();
    load_icon();
    this->setFixedSize(900,740);
    player = 0;
    create_server = new CreateServer(this);
    connect_window = new ConnectWindow(this);
    color_choose = new ColorChoose(this);
    promotion_window = new Promotion(this);
    timer = new QTimer(this);
    color = 0;
    now_player=0;
    server = new QTcpServer(this);
    socket = new QTcpSocket(this);

    connect(create_server, SIGNAL(FinishCreate()),this,SLOT(server_New_Connect()));
    connect(create_server, SIGNAL(StopConnect()),this,SLOT(delete_server()));
    connect(connect_window, SIGNAL(StartConnect()),this,SLOT(client_New_Connect()));
    connect(timer, SIGNAL(timeout()), this, SLOT(time_change()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::load_icon(){
    icon.resize(COUNT+1);
    icon[0] = nullptr;

    QPixmap* pix;
    QString dir = QCoreApplication::applicationDirPath();

    QString file = dir + "/icon/black_bishop.png";
    pix = new QPixmap(file);
    icon[BB] = pix;
    file = dir + "/icon/black_king.png";
    pix = new QPixmap(file);
    icon[BK] = pix;

    file = dir + "/icon/black_knight.png";
    pix = new QPixmap(file);
    icon[BN] = pix;

    file = dir + "/icon/black_pawn.png";
    pix = new QPixmap(file);
    icon[BP] = pix;

    file = dir + "/icon/black_queen.png";
    pix = new QPixmap(file);
    icon[BQ] = pix;

    file = dir + "/icon/black_rook.png";
    pix = new QPixmap(file);
    icon[BR] = pix;

    file = dir + "/icon/white_bishop.png";
    pix = new QPixmap(file);
    icon[WB] = pix;

    file = dir + "/icon/white_king.png";
    pix = new QPixmap(file);
    icon[WK] = pix;

    file = dir + "/icon/white_knight.png";
    pix = new QPixmap(file);
    icon[WN] = pix;

    file = dir + "/icon/white_pawn.png";
    pix = new QPixmap(file);
    icon[WP] = pix;

    file = dir + "/icon/white_queen.png";
    pix = new QPixmap(file);
    icon[WQ] = pix;

    file = dir + "/icon/white_rook.png";
    pix = new QPixmap(file);
    icon[WR] = pix;
}

void MainWindow::set_default_table(){
    now_player=0;
    selected.type = NOTHING;
    for(int i=1;i<=8;i++){
        for(int j=1;j<=8;j++)
            table[i][j].type=NOTHING;
    }
    table[1][1].type = BR;
    table[8][1].type = BR;

    table[1][8].type = WR;
    table[8][8].type = WR;

    table[2][1].type = BN;
    table[7][1].type = BN;

    table[2][8].type = WN;
    table[7][8].type = WN;

    table[3][1].type = BB;
    table[6][1].type = BB;

    table[3][8].type = WB;
    table[6][8].type = WB;

    table[4][1].type = BQ;
    table[5][1].type = BK;

    table[4][8].type = WQ;
    table[5][8].type = WK;

    for(int i=1;i<=8;i++){
        table[i][2].type = BP;
        table[i][7].type = WP;
    }

    ui->LongCastlingButton->setEnabled(true);
    ui->ShortCastlingButton->setEnabled(true);
}


void MainWindow::set_selected(int x, int y){
    int type = table[x][y].type;
    selected.type = type;
    selected.x = x;
    selected.y = y;
    if(type==BR || type==BN || type==BB ||
                    type==BK || type==BQ || type==BP){
        selected.player = 1;
    }
    else {
        selected.player = 0;
    }

}

void MainWindow::paintEvent(QPaintEvent*){
    QPainter painter(this);
    QPen pen;
    QBrush brush;
    QRect rec;

    Item temp[9][9];

    for(int i=1;i<=8;i++)
        for(int j=1;j<=8;j++){
            if(color == 1){
                temp[i][j] = table[9-i][9-j];
            }
            else {
                temp[i][j] = table[i][j];
            }
        }


    for(int i=1;i<=8;i++)
        for(int j=1;j<=8;j++){
            rec.setRect(x_begin+(i-1)*len,y_begin+(j-1)*len,len,len);
            if((i+j)%2==0){
                painter.setBrush(Qt::white);
            }
            else{
                painter.setBrush(Qt::gray);
            }
            painter.drawRect(rec);
        }

    if(selected.type != NOTHING && now_player==color){
        int x = x_begin + (selected.x-1)*len;
        int y = y_begin + (selected.y-1)*len;
        if(color==1){
            x=x_begin + (8-selected.x)*len;
            y=y_begin + (8-selected.y)*len;
        }
        rec.setRect(x,y,len,len);
        painter.setBrush(Qt::red);
        painter.drawRect(rec);
        for(int i=1;i<=8;i++)
            for(int j=1;j<=8;j++){
                x = x_begin + (i-1)*len;
                y = y_begin + (j-1)*len;
                if(color==1){
                    x=x_begin + (8-i)*len;
                    y=y_begin + (8-j)*len;
                }
                rec.setRect(x,y,len,len);
                if(move_check(selected.x,selected.y,i,j,selected.type))
                    painter.drawRect(rec);
            }
    }


    for(int i=1;i<=8;i++)
        for(int j=1;j<=8;j++){
            if(temp[i][j].type == NOTHING){
            }
            else{
                rec.setRect(x_begin+(i-1)*len+5,y_begin+(j-1)*len+5,len-10,len-10);
                int type = temp[i][j].type;
                painter.drawImage(rec,icon[type]->toImage(),icon[type]->rect());
            }
        }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if(now_player != color)
        return;
    if(event->button() == Qt::LeftButton)
        move_chess(event->x(),event->y());
}


void MainWindow::move_chess(int inx, int iny){
    int x = (inx-x_begin)/len + 1;
    int y = (iny-y_begin)/len + 1;

    if(color==1){
        x=9-x;
        y=9-y;
    }

    if(x<1 || y<1 || x>8 || y>8)
        return;


    int t = table[x][y].type;


    if(selected.type == NOTHING){
        int flag=0;
        if(t==BR || t==BN || t==BB ||
                        t==BK || t==BQ || t==BP){
            flag=1;
        }
        if(flag!=color)
            return;

        if(t == NOTHING)
            return;
        set_selected(x,y);
    }
    else{
        if(selected.x == x && selected.y == y){
            selected.type = NOTHING;
        }
        else if(move_check(selected.x,selected.y,x,y,selected.type)){
            if(table[x][y].type == BK && now_player==0){
                output_translate(selected.type, selected.x, selected.y, x, y);
                white_win();
            }
            else if(table[x][y].type == WK&&now_player==1){
                output_translate(selected.type, selected.x, selected.y, x, y);
                black_win();
            }
            if((selected.type==WK && color==0) || (selected.type==BK && color==1)){
                ui->LongCastlingButton->setEnabled(false);
                ui->ShortCastlingButton->setEnabled(false);
            }
            if(selected.type==WR || selected.type==BR){
                if(selected.x==1 && selected.y==8 && color==0){
                    ui->LongCastlingButton->setEnabled(false);
                }
                else if(selected.x==8 && selected.y==8 && color==0){
                    ui->ShortCastlingButton->setEnabled(false);
                }
                else if(selected.x==1 && selected.y==1 && color==1){
                    ui->ShortCastlingButton->setEnabled(false);
                }
                else if(selected.x==8 && selected.y==1 && color==1){
                    ui->ShortCastlingButton->setEnabled(false);
                }
            }
            int type = table[selected.x][selected.y].type;
            table[selected.x][selected.y].type = NOTHING;
            table[x][y].type = selected.type;
            selected.type = NOTHING;
            int flag = 0;
            if(y==1 || y==8)
                if(table[x][y].type == BP || table[x][y].type == WP){
                    promotion_window->set_color(color,x,y);
                    promotion_window->exec();
                    promotion(x,y,promotion_window->get_type());
                    flag = 1;
                }
            if(flag){
                output_translate(type, selected.x, selected.y, x, y, promotion_window->get_type());
            }
            else {
                output_translate(table[x][y].type, selected.x, selected.y, x, y);
            }
            if(draw_check()){
                draw();
            }
            change_now_player();
        }
        else{
            return;
        }
    }
    update();
}

bool MainWindow::move_check(int sx, int sy, int x, int y, int t){
    if(sx == x && sy == y)
        return 1;

    int player[9][9];

    for(int i=1; i<=8; i++)
        for(int j=1; j<=8; j++){
            int type = table[i][j].type;
            if(type==BR || type==BN || type==BB ||
                            type==BK || type==BQ || type==BP){
                player[i][j]=1;
            }
            else if(table[i][j].type==NOTHING){
                player[i][j]=-1;
            }
            else {
                player[i][j]=0;
            }
        }

    if(player[x][y] == player[sx][sy] && table[x][y].type!=NOTHING)
        return 0;
    int Nmove[8][2] = {{-1,-2},{1,-2},{2,-1},{2,1},{1,2},{-1,2},{-2,1},{-2,-1}};
    int Nx,Ny;

    switch (t) {
    case BR: case WR:
        if(sx!=x && sy!=y)
            return 0;
        if(sx == x){
            if(sy < y){
                for(int i=sy+1; i<y; i++){
                    if(table[x][i].type != NOTHING)
                        return 0;
                }
            }
            else {
                for(int i=sy-1; i>y; i--){
                    if(table[x][i].type != NOTHING)
                        return 0;
                }
            }
        }
        else{
            if(sx < x){
                for(int i=sx+1; i<x; i++){
                    if(table[i][y].type != NOTHING)
                        return 0;
                }
            }
            else {
                for(int i=sx-1; i>x; i--){
                    if(table[i][y].type != NOTHING)
                        return 0;
                }
            }

        }
        break;
    case BN: case WN:
        for(int i=0;i<8;i++){
            Nx = sx + Nmove[i][0];
            Ny = sy + Nmove[i][1];
            if(Nx<1 || Nx>8 || Ny<1 || Ny>8){
            }
            else if(x==Nx && y==Ny){
                return 1;
            }
        }
        return 0;
    case BB: case WB:
        if(x-y == sx-sy){
            if(x<sx){
                for(int i=1;i<sx-x;i++){
                    if(table[x+i][y+i].type != NOTHING)
                        return 0;
                }
            }
            else {
                for(int i=1;i<x-sx;i++){
                    if(table[x-i][y-i].type != NOTHING)
                        return 0;
                }
            }
            return 1;
        }
        else if(x+y == sx+sy){
            if(x<sx){
                for(int i=1;i<sx-x;i++){
                    if(table[x+i][y-i].type != NOTHING)
                        return 0;
                }
            }
            else {
                for(int i=1;i<x-sx;i++){
                    if(table[x-i][y+i].type != NOTHING)
                        return 0;
                }
            }
            return 1;
        }
        else {
            return 0;
        }
    case BK: case WK:
        if(sx-x>1 || sx-x<-1)
            return 0;
        if(sy-y>1 || sy-y<-1)
            return 0;
        return 1;
    case BQ: case WQ:
        if(move_check(sx,sy,x,y,BB) || move_check(sx,sy,x,y,BR))
            return 1;
        else
            return 0;
    case WP:
        if(sy==7){
            if(x==sx && y==6 && table[x][6].type == NOTHING)
                return 1;
            if(x==sx && y==5 && table[x][6].type==NOTHING && table[x][5].type==NOTHING)
                return 1;
            if((x-sx==1 || sx-x==1) && y==6 && player[x][y]==1)
                return 1;
        }
        else{
            if(y != sy-1)
                return 0;
            if(x==sx && table[x][y].type == NOTHING)
                return 1;
            if((x-sx==1 || sx-x==1) && player[x][y]==1)
                return 1;
        }
        return 0;
    case BP:
        if(sy==2){
            if(x==sx && y==3 && table[x][3].type == NOTHING)
                return 1;
            if(x==sx && y==4 && table[x][3].type==NOTHING && table[x][4].type==NOTHING)
                return 1;
            if((x-sx==1 || sx-x==1) && y==3 && player[x][y]==0)
                return 1;
        }
        else{
            if(y != sy+1)
                return 0;
            if(x==sx && table[x][y].type == NOTHING)
                return 1;
            if((x-sx==1 || sx-x==1) && player[x][y]==0)
                return 1;
        }
        return 0;
    }
    return 1;
}

void MainWindow::promotion(int x, int y, int type){
    int changeto;
    switch (type) {
    case 1:
        if(now_player)
            changeto = BQ;
        else {
            changeto = WQ;
        }
        break;
    case 2:
        if(now_player)
            changeto = BR;
        else {
            changeto = WR;
        }
        break;
    case 3:
        if(now_player)
            changeto = BB;
        else {
            changeto = WB;
        }
        break;
    case 4:
        if(now_player)
            changeto = BN;
        else {
            changeto = WN;
        }
        break;
    default:
        if(now_player)
            changeto = BQ;
        else {
            changeto = WQ;
        }
        break;
    }
    table[x][y].type = changeto;
    update();
}

void MainWindow::change_now_player(){
    now_player = !now_player;
    ui->lcdNumber->display(60);
    selected.type = NOTHING;
    if(now_player == color){
        timer->setInterval(1000);
        timer->start();
        ui->TurnLabel->setText(QString("Your Move"));
    }
    else{
        timer->stop();
        ui->TurnLabel->setText(QString("Opponent's Move"));
    }
}

void MainWindow::time_change(){
    ui->lcdNumber->display(ui->lcdNumber->value()-1);
    if(ui->lcdNumber->value()<=1e-6){
        if(color==1){
            white_win();
        }
        else {
            black_win();
        }
    }
}

void MainWindow::white_win(bool sent){
    if(color==1){
        QMessageBox::information(this, "Game over", "You Lose");
    }
    else{
        QMessageBox::information(this, "Game over", "You Win");
    }
    if(sent)
        socket_Write_Data(QString("White Win "));
    timer->stop();
    now_player=-1;
}

void MainWindow::black_win(bool sent){
    if(color==0){
        QMessageBox::information(this, "Game over", "You Lose");
    }
    else{
        QMessageBox::information(this, "Game over", "You Win");
    }
    if(sent)
        socket_Write_Data(QString("Black Win "));
    timer->stop();
    now_player=-1;
}

void MainWindow::draw(bool sent){
    QMessageBox::information(this, "Game over", "Draw");
    if(sent)
        socket_Write_Data(QString("Stalemate"));
    timer->stop();
}

bool MainWindow::draw_check(){
    int player[9][9];
    int king_x=0,king_y=0,op=1;
    if(now_player==1)
        op=0;
    for(int i=1;i<=8;i++)
        for(int j=1;j<=8;j++){
            int type = table[i][j].type;
            if(op==1&&type==BK){
                king_x=i;king_y=j;
            }
            if(op==0&&type==WK){
                king_x=i;king_y=j;
            }
            if(type==BK || type==BQ || type==BB ||
               type==BR || type==BN || type==BP)
                player[i][j]=1;
            else if(type==NOTHING)
                player[i][j]=-1;
            else{
                player[i][j]=0;
            }
        }
    if(!king_x)
        return false;
    if(check(king_x,king_y,op))
        return false;
    for(int i=1;i<=8;i++)
        for(int j=1;j<=8;j++){
            if(player[i][j] == op){
                if(table[i][j].type!=BK && table[i][j].type!=WK){
                    for(int m=1;m<=8;m++)
                        for(int n=1;n<=8;n++){
                            if(move_check(i,j,m,n,table[i][j].type))
                                return false;
                        }
                }
            }
        }
    for(int i=1;i<=8;i++)
        for(int j=1;j<=8;j++){
            if(i==king_x && j==king_y){

            }
            else if(i-king_x<=1 && i-king_x>=-1 && j-king_y<=1 && j-king_y>=-1){
                if(move_check(king_x,king_y,i,j,table[king_x][king_y].type) &&
                   !check(i,j,op)){
                    return false;
                }
            }
        }

    return true;
}




bool MainWindow::check(int x, int y, int king_color){
    int player[9][9];
    for(int i=1; i<=8; i++)
        for(int j=1; j<=8; j++){
            int type = table[i][j].type;
            if(type==BR || type==BN || type==BB ||
                            type==BK || type==BQ || type==BP){
                player[i][j]=1;
            }
            else if(type==NOTHING){
                player[i][j]=-1;
            }
            else {
                player[i][j]=0;
            }
        }

    for(int i=1;i<=8;i++){
        for(int j=1;j<=8;j++){
            if(table[i][j].type!=NOTHING && player[i][j]!=king_color){
                if(move_check(i,j,x,y,table[i][j].type)){
                    return 1;
                }
            }
        }
    }
    return 0;
}


void MainWindow::on_CreateButton_clicked()
{
    QList<QHostAddress> addList = QNetworkInterface::allAddresses();

    int flag=0;
    foreach(QHostAddress address,addList)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol
                        && address != QHostAddress(QHostAddress::LocalHost))
        {
            QString ip = address.toString();
            flag=1;
            create_server->set_ip(ip);
            break;
        }
    }
    if(!flag){
        create_server->set_ip("127.0.0.1");
    }

    create_server->show();
}

void MainWindow::on_ConnectButton_clicked()
{
    connect_window->show();
}

void MainWindow::on_LongCastlingButton_clicked(){
    if(now_player == 1){
        if(table[1][1].type!=BR || table[5][1].type!=BK)
            return;
        for(int i=1;i<=5;i++){
            if(check(i,1,color) || !move_check(1,1,4,1,BR))
                return;
        }
        table[3][1].type = BK;
        table[4][1].type = BR;
        table[5][1].type = NOTHING;
        table[1][1].type = NOTHING;
        selected.type=NOTHING;
        ui->LongCastlingButton->setEnabled(false);
        ui->ShortCastlingButton->setEnabled(false);
    }
    else if(now_player == 0){
        if(table[1][8].type!=WR || table[5][8].type!=WK)
            return;
        for(int i=1;i<=5;i++){
            if(check(i,8,color) || !move_check(1,8,4,8,WR))
                return;
        }
        table[3][8].type = WK;
        table[4][8].type = WR;
        table[5][8].type = NOTHING;
        table[1][8].type = NOTHING;
        selected.type=NOTHING;
        ui->LongCastlingButton->setEnabled(false);
        ui->ShortCastlingButton->setEnabled(false);
    }
    if(color==1){
        socket_Write_Data(QString("Castling E8 A8"));
    }
    else{
        socket_Write_Data(QString("Castling E1 A1"));
    }
    change_now_player();
    update();
}

void MainWindow::on_ShortCastlingButton_clicked()
{
    if(now_player == 1){
        if(table[8][1].type!=BR || table[5][1].type!=BK)
            return;
        for(int i=5;i<=7;i++){
            if(check(i,1,color) || !move_check(8,1,6,1,BR))
                return;
        }
        table[7][1].type = BK;
        table[6][1].type = BR;
        table[5][1].type = NOTHING;
        table[8][1].type = NOTHING;
        selected.type=NOTHING;
        ui->LongCastlingButton->setEnabled(false);
        ui->ShortCastlingButton->setEnabled(false);
    }
    else if(now_player == 0){
        if(table[8][8].type!=WR || table[5][8].type!=WK)
            return;
        for(int i=5;i<=7;i++){
            if(check(i,8,color) || !move_check(8,8,6,8,WR))
                return;
        }
        table[7][8].type = WK;
        table[6][8].type = WR;
        table[5][8].type = NOTHING;
        table[8][8].type = NOTHING;
        selected.type=NOTHING;
        ui->LongCastlingButton->setEnabled(false);
        ui->ShortCastlingButton->setEnabled(false);
    }
    if(color==1){
        socket_Write_Data(QString("Castling E8 H8"));
    }
    else{
        socket_Write_Data(QString("Castling E1 H1"));
    }
    change_now_player();
    update();
}

void MainWindow::on_ResignButton_clicked()
{
    if(color==1){
        white_win();
    }
    else {
        black_win();
    }
    socket_Write_Data(QString("Resign"));
}


void MainWindow::server_New_Connect(){
    delete_server();
    set_default_table();
    server->listen(create_server->get_ip(),(unsigned short)create_server->get_port());
    ui->textBrowser->append(QString("Waiting for connection..."));
    connect(server, SIGNAL(newConnection()), this, SLOT(accept_connection()));
}

void MainWindow::delete_server(){
    server->deleteLater();
    server = new QTcpServer(this);
    socket->deleteLater();
    socket = new QTcpSocket(this);
    ui->textBrowser->append("Stop Connection");
}

void MainWindow::accept_connection(){
    ui->textBrowser->clear();
    now_player=0;
    socket = new QTcpSocket(this);
    socket = this->server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()),this,SLOT(socket_Read_Data()));
    connect(socket, SIGNAL(disconnect()),socket,SLOT(deleteLater()));
    ui->textBrowser->append("Connect "+socket->peerAddress().toString());
    color_choose->exec();
    color = color_choose->get_color();
    if(color==1){
        socket_Write_Data("Black");
        ui->TurnLabel->setText(QString("Opponent's Move"));
    }
    else{
        socket_Write_Data("White");
        ui->lcdNumber->display(60);
        timer->setInterval(1000);
        timer->start();
        ui->TurnLabel->setText(QString("Your Move"));
    }
    set_default_table();
}

void MainWindow::client_New_Connect(){
    ui->textBrowser->clear();
    delete socket;
    socket = new QTcpSocket(this);
    socket->connectToHost(connect_window->get_ip(),(unsigned short)connect_window->get_port());
    now_player=0;
    connect(socket, SIGNAL(readyRead()),this,SLOT(socket_Read_Data()));
    connect(socket, SIGNAL(disconnect()),socket,SLOT(deleteLater()));
    ui->textBrowser->append("Connecting "+connect_window->get_ip().toString());
    set_default_table();
}

void MainWindow::socket_Read_Data(){
    QString info;
    info += this->socket->readAll();
    input_translate(info);
    ui->textBrowser->append(info);
}

void MainWindow::socket_Write_Data(QString command){
    if(socket == nullptr)
        return;
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(command);
    qDebug()<<array->data();
    socket->write(array->data());
    socket->flush();
    ui->textBrowser->append(command);
}



void MainWindow::input_translate(QString info){
    QStringList true_info = info.split("\n",QString::SkipEmptyParts);
    if(!true_info[0].compare("Ending")){
        true_info.pop_front();
        load_table(true_info.join("\n"));
    }
    else{
        QStringList command = info.split(" ",QString::SkipEmptyParts);
        int type=NOTHING,x1=0,y1=0,x2=0,y2=0,pro=0;
        if(command.size()==1){
            if(!command[0].compare("White")){
                color = 1;
                update();
            }
            else if(!command[0].compare("Black")){
                color = 0;
                update();
            }
            else if(!command[0].compare("Stalemate")){
                draw(0);
            }
        }
        else if(command.size()==2){
            if(!command[0].compare("White")){
                white_win(0);
            }
            else {
                black_win(0);
            }
        }
        else if(command.size()==3){
            int y = 8;
            int x = 3;
            int xR = 1;
            int x2 = 4;
            if(command[2][0] == 'H'){
                xR = 8;
                x = 7;
                x2 = 6;
            }

            if(color == 1){
                y = 8;
                table[x][y].type = WK;
                table[x2][y].type = WR;
            }
            else{
                table[x][y].type = BK;
                table[x2][y].type = BR;
            }

            table[5][y].type = NOTHING;
            table[xR][y].type = NOTHING;
            change_now_player();
        }
        else if(command.size() >= 4){
            x1 = command[2].toStdString()[0] - 'A' + 1;
            y1 = command[2].toStdString()[1] - '0';
            y1 = 9 - y1;

            x2 = command[3].toStdString()[0] - 'A' + 1;
            y2 = command[3].toStdString()[1] - '0';
            y2 = 9 - y2;

            if(!command[0].compare("Black")){
                if(!command[1].compare("Rook") || !command[1].compare("Castling"))
                    type = BR;
                else if(!command[1].compare("Knight"))
                    type = BN;
                else if(!command[1].compare("Bishop"))
                    type = BB;
                else if(!command[1].compare("King"))
                    type = BK;
                else if(!command[1].compare("Queen"))
                    type = BQ;
                else if(!command[1].compare("Pawn"))
                    type = BP;
            }
            else if(command[1].compare("White")){
                if(!command[1].compare("Rook") || !command[1].compare("Castling"))
                    type = WR;
                else if(!command[1].compare("Knight"))
                    type = WN;
                else if(!command[1].compare("Bishop"))
                    type = WB;
                else if(!command[1].compare("King"))
                    type = WK;
                else if(!command[1].compare("Queen"))
                    type = WQ;
                else if(!command[1].compare("Pawn"))
                    type = WP;
            }

            if(move_check(x1,y1,x2,y2,type)){
                table[x1][y1].type = NOTHING;
                table[x2][y2].type = type;
            }
            if(command.size()==5){
                if(!command[4].compare("Queen"))
                    pro = 1;
                else if(!command[4].compare("Rook"))
                    pro = 2;
                else if(!command[4].compare("Bishop"))
                    pro = 3;
                else if(!command[4].compare("Knight"))
                    pro = 4;
                promotion(x2,y2,pro);
            }
            change_now_player();
        }
    }
    if(now_player == color){
        ui->lcdNumber->display(60);
        timer->setInterval(1000);
        timer->start();
        ui->TurnLabel->setText("Your Move");
    }
    else{
        ui->lcdNumber->display(60);
        timer->stop();
        ui->TurnLabel->setText("Opponent's Move");
    }
    update();
}

void MainWindow::output_translate(int type, int x1, int y1, int x2, int y2, int pro){
    QString command;
    if(color == 1)
        command = "Black ";
    else {
        command = "White ";
    }

    switch (type) {
    case BR: case WR:
        command += "Rook ";
        break;
    case BN: case WN:
        command += "Knight ";
        break;
    case BB: case WB:
        command += "Bishop ";
        break;
    case BK: case WK:
        command += "King ";
        break;
    case BQ: case WQ:
        command += "Queen ";
        break;
    case BP: case WP:
        command += "Pawn ";
        break;
    default:
        break;
    }

    char a1 = (char)('A'+x1-1);
    command += a1 + QString::number(9-y1) + " ";

    char a2 = (char)('A'+x2-1);
    command += a2 + QString::number(9-y2) + " ";

    if(pro>=1){
        switch (pro) {
        case 1:
            command += "Queen ";
            break;
        case 2:
            command += "Rook ";
            break;
        case 3:
            command += "Bishop ";
            break;
        case 4:
            command += "Knight ";
            break;
        default:
            break;
        }
    }
    socket_Write_Data(command);
}


void MainWindow::load_table(QString info){
    for(int i=1;i<=8;i++)
        for(int j=1;j<=8;j++)
            table[i][j].type = NOTHING;

    QStringList table_info = info.split("\n", QString::SkipEmptyParts);
    if(!table_info[0].compare("white")){
        now_player=0;
        int i=1;
        while(table_info[i].compare("black") != 0){
            int type=NOTHING;
            QStringList command = table_info[i].split(" ", QString::SkipEmptyParts);
            if(!command[0].compare("king")){
                type = WK;
            }
            else if(!command[0].compare("queen")){
                type = WQ;
            }
            else if(!command[0].compare("rook")){
                type = WR;
            }
            else if(!command[0].compare("bishop")){
                type = WB;
            }
            else if(!command[0].compare("knight")){
                type = WN;
            }
            else if(!command[0].compare("pawn")){
                type = WP;
            }
            for(int j=0;j<command[1].toInt();j++){
                int x = command[j+2].toStdString()[0] -'a'+1;
                int y = command[j+2].toStdString()[1] -'0';
                table[x][9-y].type = type;
            }
            i++;
        }
        i++;
        while(i<table_info.size()){
            int type=NOTHING;
            QStringList command = table_info[i].split(" ", QString::SkipEmptyParts);
            if(!command[0].compare("king")){
                type = BK;
            }
            else if(!command[0].compare("queen")){
                type = BQ;
            }
            else if(!command[0].compare("rook")){
                type = BR;
            }
            else if(!command[0].compare("bishop")){
                type = BB;
            }
            else if(!command[0].compare("knight")){
                type = BN;
            }
            else if(!command[0].compare("pawn")){
                type = BP;
            }

            for(int j=0;j<command[1].toInt();j++){
                int x = command[j+2].toStdString()[0] -'a'+1;
                int y = command[j+2].toStdString()[1] -'0';
                table[x][9-y].type = type;
            }
            i++;
        }
    }
    else if(!table_info[0].compare("black")){
        now_player=1;
        int i=1;
        while(table_info[i].compare("white") != 0){
            int type=NOTHING;
            QStringList command = table_info[i].split(" ", QString::SkipEmptyParts);
            if(!command[0].compare("king")){
                type = BK;
            }
            else if(!command[0].compare("queen")){
                type = BQ;
            }
            else if(!command[0].compare("rook")){
                type = BR;
            }
            else if(!command[0].compare("bishop")){
                type = BB;
            }
            else if(!command[0].compare("knight")){
                type = BN;
            }
            else if(!command[0].compare("pawn")){
                type = BP;
            }
            for(int j=0;j<command[1].toInt();j++){
                int x = command[j+2].toStdString()[0] -'a'+1;
                int y = command[j+2].toStdString()[1] -'0';
                table[x][9-y].type = type;
            }
            i++;
        }
        i++;
        while(i<table_info.size()){
            int type=NOTHING;
            QStringList command = table_info[i].split(" ", QString::SkipEmptyParts);
            if(!command[0].compare("king")){
                type = WK;
            }
            else if(!command[0].compare("queen")){
                type = WQ;
            }
            else if(!command[0].compare("rook")){
                type = WR;
            }
            else if(!command[0].compare("bishop")){
                type = WB;
            }
            else if(!command[0].compare("knight")){
                type = WN;
            }
            else if(!command[0].compare("pawn")){
                type = WP;
            }

            for(int j=0;j<command[1].toInt();j++){
                int x = command[j+2].toStdString()[0] -'a'+1;
                int y = command[j+2].toStdString()[1] -'0';
                table[x][9-y].type = type;
            }
            i++;
        }
    }
    if(color==now_player){
        ui->lcdNumber->display(60);
        timer->setInterval(1000);
        timer->start();
        ui->textBrowser->setText("Your Move");
    }
    else{
        ui->lcdNumber->display(60);
        timer->stop();
        ui->textBrowser->setText("Opponent's Move");
    }
    ui->LongCastlingButton->setEnabled(true);
    ui->ShortCastlingButton->setEnabled(true);
    update();
}

void MainWindow::on_LoadButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"Open Document",QString(),QString("Text File(*.txt)"));
    QFile file(filename);
    if(file.open(QFile::ReadOnly | QFile::Text)){
        QTextStream in(&file);
        QString table = in.readAll();
        load_table(table);
        color_choose->exec();
        color = color_choose->get_color();
        socket_Write_Data("Ending\n"+table);
        if(color==1){
            socket_Write_Data("Black");
        }
        else {
            socket_Write_Data("White");
        }
    }
    else {
        ui->textBrowser->append(QString("File Not Found"));
    }
}

QString MainWindow::find_all(int type){
    QString chess_type;
    int num=0;
    switch (type) {
    case WK: case BK:
        chess_type = "king ";
        break;
    case WQ: case BQ:
        chess_type = "queen ";
        break;
    case WB: case BB:
        chess_type = "bishop ";
        break;
    case WN: case BN:
        chess_type = "knight ";
        break;
    case WR: case BR:
        chess_type = "rook ";
        break;
    case WP: case BP:
        chess_type = "pawn ";
        break;
    }
    QString pos;
    for(int i=1;i<=8;i++)
        for(int j=1;j<=8;j++){
            if(table[i][j].type==type){
                char x = 'a'+(char)(i-1);
                pos +=x+QString::number(9-j)+" ";
                num++;
            }
        }
    if(num>0)
        return chess_type+QString::number(num)+" "+pos+"\n";
    else {
        return "";
    }
}

QString MainWindow::save_table(){
    QString command_w,command_b;
    command_b = "black\n";
    command_b+=find_all(BK);
    command_b+=find_all(BQ);
    command_b+=find_all(BB);
    command_b+=find_all(BN);
    command_b+=find_all(BR);
    command_b+=find_all(BP);

    command_w = "white\n";
    command_w+=find_all(WK);
    command_w+=find_all(WQ);
    command_w+=find_all(WB);
    command_w+=find_all(WN);
    command_w+=find_all(WR);
    command_w+=find_all(WP);

    if(now_player==1){
        return command_b+command_w;
    }
    else {
        return command_w+command_b;
    }
}

void MainWindow::on_SaveButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,"Save File",QString(),QString("Text File(*.txt)"));
    QFile file(filename);
    if(file.open(QFile::WriteOnly | QFile::Truncate)){
        QTextStream out(&file);
        out << save_table();
        ui->textBrowser->append("Save As:" + filename);
    }
    else {
        ui->textBrowser->append(QString("Save Failed"));
    }
}
