#ifndef DMFB_H
#define DMFB_H

#include <QObject>
#include <vector>
#include "command.h"
using std::vector;

class State;

struct Item{
   int id;
   int type;
   int size;
   Item(){
       id=0;
       type=0;
       size=5;
   }
   Item& operator=(const Item& a){
       if(this == &a)
           return *this;
       id = a.id;
       type = a.type;
       return *this;
   }
};

class DMFB : public QObject
{
    Q_OBJECT
public:
    enum{
        TABLE_SMALL_ERROR = 0,
        TABLE_BIG_ERROR,
        TOO_MANY_INPUT,
        FILE_NOT_FOUND,
        COMMAND_ERROR
    };
    enum{
        NOTHING = 0,
        INPUT,
        OUTPUT,
        WASH_PUT,
        INPUT_DROP,
        OUTPUT_DROP,
        NORMAL,
        MERGE,
        MERGE_UP,
        MERGE_DOWN,
        MERGE_LEFT,
        MERGE_RIGHT,
        TRACE
    };
    const int length = 50;
   int id_count[100];

    explicit DMFB(QObject *parent = nullptr);
    ~DMFB();

    int row=-1;
    int col=-1;
    int in_num=0;
    int out_num=1;
    int wash_num=0;
    QString filename;
    vector<vector<Item>> table;
    vector<vector<Item>> next_table;
    vector<vector<Item>> copy;
    vector<vector<Command>>time_command;

    void set_state(State* s);
    int get_state();

    void undo();
    void redo();
    void show();

private:
    State* state;
    int time_now, life_span;
    void check();

signals:
    void MainUpdate();
    void Error(int);

public slots:
    void ReceiveData(int, int, int, QString);
    void next_state();
};

#endif // DMFB_H
