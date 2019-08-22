#include "dmfb.h"
#include "state.h"
#include <QMouseEvent>
#include "mainwindow.h"


DMFB::DMFB(QObject *p) : QObject(p){
    state = new StartState(this);
    memset(id_count, 0, sizeof(id_count));
}

DMFB::~DMFB(){
   delete state;
}


void DMFB::ReceiveData(int r, int l, int num, QString f){
    row = r;
    col = l;
    in_num = num;
    out_num = 1;
    filename = f;
    vector<Item> temp(row+2);
    table.assign(col+2, temp);
    copy.assign(col+2, temp);
    next_table.assign(col+2, temp);

    this->set_state(new SetInputState(this));
    if(row<=3 && col<=3 && (row!=-1 || col!=-1))
        emit Error(TABLE_SMALL_ERROR);
    else if(row>=16 || col >=31)
        emit Error(TABLE_BIG_ERROR);
    else if(in_num > row*2 + col*2 - 1 && in_num)
        emit Error(TOO_MANY_INPUT);
    else
        emit MainUpdate();

    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text))
        emit Error(DMFB::FILE_NOT_FOUND);
}

void DMFB::next_state(){
    state->next();
}

void DMFB::set_state(State *s){
    State* old = state;
    state = s;
    delete old;
}

int DMFB::get_state(){
    return state->getstate();
}

void DMFB::undo(){
    if(time_now>0)
        time_now--;
}

void DMFB::redo(){
    if(time_now<life_span)
        time_now++;
}

void DMFB::show(){
    table = copy;
    for(int i=0; i<=time_now; i++){
        for(int j=0; j<(int)time_command[i].size(); j++)
            time_command[i][j].execute(this);
    }
    next_table = table;
    for(int i=0; i<(int)time_command[time_now+1].size(); i++)
        time_command[time_now+1][i].execute(this);
    vector<vector<Item>> temp = table;
    table = next_table;
    next_table  = temp;
    check();
    emit MainUpdate();
}

void DMFB::check(){
    for(int i=1;i<=col;i++)
        for(int j=1;j<=row;j++){
            switch (table[i][j].type) {
            default:
                break;
            }
        }
}

