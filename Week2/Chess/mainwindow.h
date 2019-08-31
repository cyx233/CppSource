#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPaintEvent>
#include <QPixmap>
#include <QMouseEvent>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include "createserver.h"
#include "colorchoose.h"
#include "promotion.h"
#include "connectwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum type{
        NOTHING,
        WB,//Bishop
        WK,//King
        WN,//Knight
        WP,//Pawn
        WQ,//Queen
        WR,//Rook
        BB,
        BK,
        BN,
        BP,
        BQ,
        BR,
        COUNT
    };

    struct Item{
        int type;
        Item():type(NOTHING){}
    };

    struct SelectItem:public Item{
        int x;
        int y;
        int player;
        SelectItem():Item(),x(0),y(0){}
    };

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void server_New_Connect();
    void client_New_Connect();
    void socket_Read_Data();
    void accept_connection();
    void delete_server();
    void time_change();


protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);

private slots:
    void on_CreateButton_clicked();
    void on_LongCastlingButton_clicked();
    void on_ShortCastlingButton_clicked();
    void on_ConnectButton_clicked();
    void on_ResignButton_clicked();
    void on_LoadButton_clicked();
    void on_SaveButton_clicked();

private:
    const int len = 80;
    const int x_begin = 50;
    const int y_begin = 50;
    int color;

    Ui::MainWindow *ui;

    QVector<QPixmap*>icon;
    Item table[9][9];

    SelectItem selected;
    int player;
    int now_player;

    CreateServer* create_server;
    ConnectWindow* connect_window;
    ColorChoose* color_choose;
    QTcpServer* server;
    QTcpSocket* socket;
    Promotion* promotion_window;
    QTimer* timer;


    void set_default_table();
    void set_selected(int x, int y);
    void load_icon();

    void move_chess(int inx, int iny);
    bool move_check(int sx, int sy, int x, int y, int t=NOTHING);
    void white_win(bool sent=1);
    void black_win(bool sent=1);
    void draw(bool sent=1);
    bool draw_check();
    bool check(int x, int y, int color);
    void promotion(int, int, int);
    void change_now_player();
    void load_table(QString);
    QString save_table();
    QString find_all(int type);

    void init_server();
    void input_translate(QString);
    void output_translate(int type, int x1, int y1, int x2, int y2, int pro=-1);
    void socket_Write_Data(QString command);
};

#endif // MAINWINDOW_H
