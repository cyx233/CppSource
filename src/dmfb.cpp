#include "dmfb.h"
#include "state.h"
#include <QMouseEvent>
#include <QPair>
#include <QQueue>
#include <QCoreApplication>
#include <QTime>
#include "receiver.h"
#include "mainwindow.h"
#include "sound.h"
#include <unistd.h>

using namespace std;


DMFB::DMFB(QObject *p) : QObject(p){
    state = new StartState(this);
    memset(id_count, 0, sizeof(id_count));
    for(int i=0;i<(int)time_command.size();i++)
        for(int j=0;j<(int)time_command[i].size();j++)
            delete time_command[i][j];
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
    vector<int>temp1;
    vector<vector<int>>temp2(row+2,temp1);
    trace_exist.assign(col+2,temp2);

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

void DMFB::set_default_wash(){
    if(table[0][row].type == NOTHING){
        table[0][row].type = WASH_PUT;
        wash_num = 0;
    }
    else if(table[1][row+1].type == NOTHING){
        table[1][row+1].type = WASH_PUT;
        wash_num = 0;
    }
    else{
        for(int i=row; i>=1; i--){
            if(table[0][i].type == NOTHING){
                table[0][i].type = WASH_PUT;
                wash_num = 0;
                break;
            }
            else if(table[col+1][i].type == NOTHING){
                table[col+1][i].type = WASH_PUT;
                wash_num = 0;
                break;
            }
        }
    }

    if(wash_num){
        for(int i=1; 1<=col; i++){
            if(table[i][row+1].type == NOTHING){
                table[i][row+1].type = WASH_PUT;
                wash_num = 0;
                break;
            }
            else if(table[i][0].type == NOTHING){
                table[i][0].type = WASH_PUT;
                wash_num = 0;
                break;
            }
        }
    }
}


void DMFB::set_default_waste(){
    if(table[col][0].type == NOTHING){
        table[col][0].type = WASTE;
        waste_num = 0;
    }
    else if(table[col+1][1].type == NOTHING){
        table[col+1][1].type = WASTE;
        waste_num = 0;
    }
    else{
        for(int i=1; i<=row; i++){
            if(table[col+1][i].type == NOTHING){
                table[col+1][i].type = WASTE;
                waste_num = 0;
                break;
            }
            else if(table[col+1][i].type == NOTHING){
                table[col+1][i].type = WASTE;
                wash_num = 0;
                break;
            }
        }
    }

    if(waste_num){
        for(int i=col; i>=1; i--){
            if(table[i][0].type == NOTHING){
                table[i][0].type = WASTE;
                wash_num = 0;
                break;
            }
            else if(table[i][row+1].type == NOTHING){
                table[i][0].type = WASTE;
                wash_num = 0;
                break;
            }
        }
    }

    if(waste_num || wash_num)
        emit Error(NO_WASH);
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
    memset(id_count, 0, sizeof(id_count));
    for(int i=0; i<=time_now; i++){
        for(int j=0; j<(int)time_command[i].size(); j++){
            if(!time_command[i][j]->execute(this))
                return;
            if(i == time_now){
                time_command[i][j]->sound();
            }
        }
    }
    if(static_check())
        return;
    next_table = table;
    if(time_now == life_span){
        emit MainUpdate();
        return;
    }
    for(int i=0; i<(int)time_command[time_now+1].size(); i++)
        if(!time_command[time_now+1][i]->execute(this))
            return;
    vector<vector<Item>> temp = table;
    table = next_table;
    next_table  = temp;
    if(dynamic_check()){
        table = next_table;
        return;
    };
    emit MainUpdate();
}

bool DMFB::static_check(){
    int d[8][2] = {{-1,0},{0,1},{1,0},{0,-1},{-1,-1},{-1,1},{1,1},{1,-1}};
    for(int i=1;i<=col;i++)
        for(int j=1;j<=row;j++){
            int flag;
            switch (table[i][j].type) {
            case INPUT_DROP:
                flag = 0;
                for(int k=0; k<4; k++){
                    int x = i+d[k][0];
                    int y = j+d[k][1];
                    if(table[x][y].type == INPUT){
                        flag = 1;
                    }
                }
                if(!flag){
                    emit Error(RUN_ERROR);
                    return 1;
                }
                table[i][j].type = NORMAL;
                break;
            case OUTPUT_DROP:
                flag = 0;
                for(int k=0; k<4; k++){
                    int x = i+d[k][0];
                    int y = j+d[k][1];
                    if(table[x][y].type == OUTPUT){
                        flag = 1;
                    }
                }
                if(!flag){
                    emit Error(RUN_ERROR);
                    return 1;
                }
                table[i][j].type = NOTHING;
                break;
            case NORMAL:
                flag = 0;
                for(int k=0; k<8; k++){
                    int x = i+d[k][0];
                    int y = j+d[k][1];
                    if(x<1 || y<1 || x>col || y>row)
                        continue;
                    if(table[x][y].type != NOTHING && table[x][y].type != TRACE &&
                                    table[x][y].type != OUTPUT_DROP){
                        flag = 1;
                        break;
                    }
                }
                if(flag){
                    emit Error(RUN_ERROR);
                    return 1;
                }
                break;
            case MERGE:
                flag = 0;
                for(int k=0; k<8; k++){
                    int x = i+d[k][0];
                    int y = j+d[k][1];
                    if(x<1 || y<1 || x>col || y>row)
                        continue;
                    if(table[x][y].type != NOTHING && table[x][y].type != TRACE &&
                                    table[x][y].type != OUTPUT_DROP){
                        flag ++;
                    }
                }
                if(flag>=2){
                    emit Error(RUN_ERROR);
                    return 1;
                }
                break;

            case MERGE_H:
                flag = 0;
                for(int k=0; k<8; k++){
                    int x = i+d[k][0];
                    int y = j+d[k][1];
                    if(x<1 || y<1 || x>col || y>row)
                        continue;
                    if(table[x][y].type != NOTHING && table[x][y].type != TRACE &&
                                    table[x][y].type != OUTPUT_DROP){
                        flag ++;
                    }
                }
                if(flag>=3){
                    emit Error(RUN_ERROR);
                    return 1;
                }
                break;

            case MERGE_V:
                flag = 0;
                for(int k=0; k<8; k++){
                    int x = i+d[k][0];
                    int y = j+d[k][1];
                    if(x<1 || y<1 || x>col || y>row)
                        continue;
                    if(table[x][y].type != NOTHING && table[x][y].type != TRACE &&
                                    table[x][y].type != OUTPUT_DROP){
                        flag ++;
                    }
                }
                if(flag>=3){
                    emit Error(RUN_ERROR);
                    return 1;
                }
                break;
            default:
                break;
            }
        }
    return 0;
}

bool DMFB::dynamic_check(){
    int d[8][2] = {{-1,0},{0,1},{1,0},{0,-1},{-1,-1},{-1,1},{1,1},{1,-1}};
    for(int i=1;i<=col;i++)
        for(int j=1;j<=row;j++){
            bool flag;
            switch (table[i][j].type) {
            case INPUT_DROP:
                table[i][j].type = NORMAL;
                break;
            case OUTPUT_DROP:
                table[i][j].type = NOTHING;
                break;
            case NORMAL:
                flag = 0;
                for(int k=0; k<8; k++){
                    int x = i+d[k][0];
                    int y = j+d[k][1];
                    if(x<1 || y<1 || x>col || y>row)
                        continue;
                    if(next_table[x][y].type == NORMAL){
                        if(next_table[x][y].id != table[i][j].id){
                            flag=1;
                            break;
                        }
                    }
                }
                if(flag){
                    emit Error(RUN_ERROR);
                    return 1;
                }
                break;
            default:
                break;
            }
        }
    return 0;
}

void DMFB::begin(){
    time_now = 0;
    for(int i=0;i<(int)time_command.size();i++)
        for(int j=0;j<(int)time_command[i].size();j++)
            delete time_command[i][j];
    memset(id_count, 0, sizeof(id_count));
    time_command.clear();
    colors.clear();
    Receiver receiver(this);
    if(!receiver.get_command()){
        return;
    }
    life_span = (int)time_command.size()-1;
    show();
}
struct Point{
    int type=0;
    bool visited = 0;
    QPoint pre,next;
    Point(){
        pre = QPoint(0,0);
        next = QPoint(0,0);
    }
};

void DMFB::wash(){
    enum{
        ROAD=0,
        WALL,
        COIN,
        DROP,
    };
    int inx=0,iny=0,outx=0,outy=0;
    int move[8][2] = {{-1,0},{0,1},{1,0},{0,-1},{-1,-1},{-1,1},{1,1},{1,-1}};
    int coin=0;

    emit NextLock(1);

    vector<Point> temp(row+2, Point());
    maze.assign(col+2, temp);

    for(int i=0; i<=row+1; i++)
        for(int j=0; j<=col+1; j++){
            maze[j][i].pre.setX(0);
            maze[j][i].pre.setY(0);
            maze[j][i].next.setX(0);
            maze[j][i].next.setY(0);
            maze[j][i].type = ROAD;
            int clean = 0;
            switch (table[j][i].type) {
            case WASH_PUT:
                if(j==0){
                    inx = j+1;
                    iny = i;
                }
                else if(j==col+1){
                    inx = j-1;
                    iny = i;
                }
                if(i==0){
                    inx = j;
                    iny = i+1;
                }
                else if(i==row+1){
                    inx = j;
                    iny = i-1;
                }
                break;
            case WASTE:
                if(j==0){
                    outx = j+1;
                    outy = i;
                }
                else if(j==col+1){
                    outx = j-1;
                    outy = i;
                }
                if(i==0){
                    outx = j;
                    outy = i+1;
                }
                else if(i==row+1){
                    outx = j;
                    outy = i-1;
                }
                break;
            case INPUT_DROP:
                maze[j][i].type = DROP;
                break;
            case NORMAL:
                maze[j][i].type = DROP;
                break;
            case MERGE:
                maze[j][i].type = DROP;
                break;
            case MERGE_H:
                maze[j][i].type = DROP;
                break;
            case MERGE_V:
                maze[j][i].type = DROP;
                break;
            default:
                clean = 1;
                break;
            }
            if(table[j][i].trace.size() && clean){
                for(int k=(int)trace_exist[j][i].size()-1;k>=0;k--)
                    if(trace_exist[j][i][k]){
                        maze[j][i].type = COIN;
                        break;
                    }
            }
            if(table[j][i].is_barrier && maze[j][i].type != DROP)
                maze[j][i].type = WALL;
        }

    for(int i=0; i<=row+1; i++)
        for(int j=0; j<=col+1; j++){
            if(j<1 || i<1 || j>col || i>row){
                if(maze[j][i].type == ROAD)
                    maze[j][i].type = WALL;
            }
            else if(maze[j][i].type == DROP){
                for(int k=0;k<8;k++){
                    int x = j+move[k][0];
                    int y = i+move[k][1];
                    if(x>=1 && x<=col && y>=1 && y<=row){
                        maze[x][y].type = WALL;
                    }

                }
            }

        }

    for(int i=1; i<=row; i++)
        for(int j=1; j<=col; j++){
            if(maze[j][i].type == COIN){
                coin = 1;
                break;
            }
        }
    if(!coin){
        emit NextLock(0);
        return;
    }

    QQueue<QPoint> list;
    int exist = 0;
    while(1){
        int findx=0,findy=0;
        int curx=0,cury=0;
        QPoint zero(0,0);
        QPoint wash(inx,iny);
        for(int i=1; i<=row; i++)
            for(int j=1; j<=col; j++){
                maze[j][i].pre = zero;
                maze[j][i].next = zero;
                maze[j][i].visited = 0;
            }

        list.clear();
        if(maze[inx][iny].type != WALL && maze[inx][iny].type != DROP){
            list.push_back(wash);
            maze[inx][iny].visited = 1;
        }

        while(list.size()){
            wash = list.head();
            curx = wash.x();
            cury = wash.y();
            for(int i=0; i<4; i++){
                int dx = curx + move[i][0];
                int dy = cury + move[i][1];
                if(dx<1 || dy<1 || dx>col || dy>row || maze[dx][dy].visited)
                    continue;
                if(maze[dx][dy].type != WALL && maze[dx][dy].type != DROP){
                    maze[dx][dy].pre = wash;
                    list.push_back(QPoint(dx,dy));
                    maze[dx][dy].visited=1;
                    if(maze[dx][dy].type == COIN){
                        if(trace_exist[dx][dy].back()){
                            findx = dx; findy = dy;
                        }
                        if(maze[outx][outy].pre.x())
                            break;
                    }
                }
            }
            list.pop_front();
        }

        if(!maze[outx][outy].pre.x()){
            emit NextLock(0);
            return;
        }

        if(findx){
            maze[findx][findy].type = ROAD;
            curx=findx; cury=findy;
            exist = 1;
        }
        else if(exist){
            curx=outx; cury=outy;
        }
        else {
            emit NextLock(0);
            return;
        }

        while(maze[curx][cury].pre.x()){
            int tempx = curx, tempy = cury;
            curx = maze[tempx][tempy].pre.x();
            cury = maze[tempx][tempy].pre.y();
            maze[curx][cury].next = QPoint(tempx,tempy);
        }

        int washx = inx, washy = iny;


        while(washx){
            curx = washx; cury = washy;
            wash_update(washx, washy);
            washx = maze[curx][cury].next.x();
            washy = maze[curx][cury].next.y();
            QTime t;
            t.start();
            while(t.elapsed()<75)
                QCoreApplication::processEvents();
        }

        if(findx){
            inx = findx; iny = findy;
        }
        else{
            wash_update(-outx,outy);
            emit NextLock(0);
            return;
        }
    }
}


void DMFB::wash_update(int x, int y){
    if(x<0){
        table[-x][y].type = NOTHING;
        emit MainUpdate();
        return;
    }
    for(int i=1;i<=col;i++)
        for(int j=1;j<=row;j++)
            if(table[i][j].type == WASH_DROP)
                table[i][j].type = NOTHING;
    for(int i=0; i<(int)table[x][y].trace.size(); i++)
        trace_exist[x][y][i] = 0;
    table[x][y].type = WASH_DROP;
    emit MainUpdate();
}

void DMFB::wash_check(){
    for(int i=1; i<=col; i++)
        for(int j=1; j<=row; j++){
            switch (table[i][j].type) {
            case DMFB::TRACE:
                break;
            case DMFB::NOTHING:
                break;
            case DMFB::WASH_PUT:
                break;
            default:
                for(int k=(int)trace_exist[i][j].size()-1; k>=0; k--){
                    if(trace_exist[i][j][k]){
                        if(table[i][j].trace[k] != table[i][j].id)
                            emit Error(DMFB::WASH_ERROR);
                    }
                }
                break;
            }
        }
}
