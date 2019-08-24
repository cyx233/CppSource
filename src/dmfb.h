#ifndef DMFB_H
#define DMFB_H

#include <QObject>
#include <QColor>
#include <vector>
#include "command.h"
using std::vector;

class State;
struct Point;

struct Item{
   int id;
   int type;
   double size;
   int is_barrier;
   vector<int>trace;
   Item(){
       id=0;
       type=0;
       size=15;
       is_barrier=0;
   }
};

class DMFB : public QObject
{
    Q_OBJECT
public:
    enum error_type{
        TABLE_SMALL_ERROR = 0,
        TABLE_BIG_ERROR,
        TOO_MANY_INPUT,
        FILE_NOT_FOUND,
        COMMAND_ERROR,
        RUN_ERROR,
        OUT_OF_CHIP,
        NO_WASH,
        WASH_ERROR
    };
    enum item_type{
        NOTHING = 0,
        INPUT,
        OUTPUT,
        WASH_PUT,
        WASTE,
        INPUT_DROP,
        OUTPUT_DROP,
        WASH_DROP,
        NORMAL,
        MERGE,
        MERGE_V,
        MERGE_H,
        TRACE,
    };
    const int length = 50;

    int id_count[20];
    vector<QColor> colors;

    explicit DMFB(QObject *parent = nullptr);
    ~DMFB();

    int row=-1;
    int col=-1;
    int in_num=0;
    int out_num=1;
    int wash_num=1;
    int waste_num=1;
    int wash_on=0;

    QString filename;
    vector<vector<Item>> table;
    vector<vector<Item>> next_table;
    vector<vector<Item>> copy;
    vector<vector<Command*>>time_command;
    vector<vector<vector<int>>>trace_exist;

    void set_state(State* s);
    int get_state();
    void set_default_wash();
    void set_default_waste();
    void begin();
    int get_time(){return time_now;}
    void set_time(int a){time_now = a;}

    void wash_check();

    void undo();
    void redo();
    void show();
    void wash();


private:
    State* state;
    int time_now, life_span;
    bool static_check();
    bool dynamic_check();
    void wash_update(int x, int y);
    void wash_show();
    vector<vector<Point>> maze;

signals:
    void MainUpdate();
    void Error(int);
    void NextLock(int);

public slots:
    void ReceiveData(int, int, int, QString);
    void next_state();
};

#endif // DMFB_H
