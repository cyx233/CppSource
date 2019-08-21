#include "dmfb.h"
#include "state.h"
#include <QMouseEvent>
#include "mainwindow.h"


DMFB::DMFB(QObject *p) : QObject(p){
    state = new StartState(this);
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

    this->set_state(new SetInputState(this));
    if(row<=3 && col<=3 && (row!=-1 || col!=-1))
        emit Error(TABLE_SMALL_ERROR);
    else if(row>=16 || col >=31)
        emit Error(TABLE_BIG_ERROR);
    else if(in_num > row*2 + col*2 - 1 && in_num)
        emit Error(TOO_MANY_INPUT);
    else
        emit MainUpdate();
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

void DMFB::doCommand(Command* command){
    raw_command_list.push_back(*command);
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
    for(int i=0; i<time_now; i++){
        for(int j=0; j<time_command[i].size(); j++)
            time_command[i][j].execute(this);
    }
    emit MainUpdate();
}

