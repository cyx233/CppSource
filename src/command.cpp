#include <QtMath>
#include <QColor>
#include <ctime>
#include "dmfb.h"
#include "command.h"

Command::Command(){
    srand((unsigned int)time(nullptr));
}

Command::~Command(){
    delete my_sound;
}

void give_color(DMFB* dmfb){
    int r=0,g=0,b=0;
    while(!r && !g && !b){
        r = rand()%256; g=rand()%256; b=rand()%256;
    }
    dmfb->colors.push_back(QColor(r,g,b));
}

bool Command::execute(DMFB *dmfb){
    int temp = dmfb->row;
    dmfb->row = 0;
    dmfb->row = temp;
    return 1;
}

bool InputCommand::execute(DMFB* dmfb){
    if(x1>dmfb->col || x1<1 || y1>dmfb->row || y1<1){
        dmfb->Error(DMFB::OUT_OF_CHIP);
        return 0;
    }

    if(dmfb->table[x1][y1].type != DMFB::NOTHING && dmfb->table[x1][y1].type != DMFB::TRACE &&
       dmfb->table[x1][y1].type != DMFB::OUTPUT_DROP){
        dmfb->Error(DMFB::RUN_ERROR);
        return 0;
    }
    dmfb->table[x1][y1].type = DMFB::INPUT_DROP;
    dmfb->table[x1][y1].id = dmfb->id_count[DMFB::NORMAL];
    dmfb->table[x1][y1].size = 15;
    if((int)dmfb->colors.size()-1 < dmfb->id_count[DMFB::NORMAL]){
        give_color(dmfb);
    }
    dmfb->id_count[DMFB::NORMAL]++;
    return 1;
}

bool MoveCommand::execute(DMFB *dmfb){
    if(x1>dmfb->col || x1<1 || y1>dmfb->row || y1<1){
        dmfb->Error(DMFB::OUT_OF_CHIP);
        return 0;
    }

    if(x2>dmfb->col || x2<1 || y2>dmfb->row || y2<1){
        dmfb->Error(DMFB::OUT_OF_CHIP);
        return 0;
    }

    dmfb->table[x1][y1].type = DMFB::TRACE;
    dmfb->table[x1][y1].id = dmfb->table[x1][y1].id;

    dmfb->table[x1][y1].trace.push_back(dmfb->table[x1][y1].id);
    if(dmfb->trace_exist[x1][y1].size()<dmfb->table[x1][y1].trace.size())
        dmfb->trace_exist[x1][y1].push_back(1);

    dmfb->table[x2][y2].type = DMFB::NORMAL;
    dmfb->table[x2][y2].id = dmfb->table[x1][y1].id;
    dmfb->table[x2][y2].size = dmfb->table[x1][y1].size;
    return 1;
}

bool SplitCommand::execute(DMFB *dmfb){
    if(x1>dmfb->col || x1<1 || y1>dmfb->row || y1<1){
        dmfb->Error(DMFB::OUT_OF_CHIP);
        return 0;
    }
    if(x2>dmfb->col || x2<1 || y2>dmfb->row || y2<1){
        dmfb->Error(DMFB::OUT_OF_CHIP);
        return 0;
    }
    if(x3>dmfb->col || x3<1 || y3>dmfb->row || y3<1){
        dmfb->Error(DMFB::OUT_OF_CHIP);
        return 0;
    }

    if(dmfb->table[x1][y1].type != DMFB::NORMAL){
        dmfb->Error(DMFB::OUT_OF_CHIP);
        return 0;
    }

    if(x2 != x1){
        dmfb->table[x1][y1].type = DMFB::MERGE_H;
        dmfb->table[x2][y2].type = DMFB::MERGE;
        dmfb->table[x3][y3].type = DMFB::MERGE;
    }
    else if(y2 != y1){
        dmfb->table[x1][y1].type = DMFB::MERGE_V;
        dmfb->table[x2][y2].type = DMFB::MERGE;
        dmfb->table[x3][y3].type = DMFB::MERGE;
    }
    return 1;
}

bool SplitCommandFinish::execute(DMFB *dmfb){

    dmfb->table[x1][y1].type = DMFB::TRACE;
    dmfb->table[x1][y1].trace.push_back(dmfb->table[x1][y1].id);
    if(dmfb->trace_exist[x1][y1].size()<dmfb->table[x1][y1].trace.size())
        dmfb->trace_exist[x1][y1].push_back(1);

    dmfb->table[x2][y2].type = DMFB::NORMAL;
    dmfb->table[x3][y3].type = DMFB::NORMAL;

    dmfb->table[x2][y2].id = dmfb->id_count[DMFB::NORMAL];
    if((int)dmfb->colors.size()-1 < dmfb->id_count[DMFB::NORMAL]){
        give_color(dmfb);
    }
    dmfb->id_count[DMFB::NORMAL]++;

    dmfb->table[x3][y3].id = dmfb->id_count[DMFB::NORMAL];
    if((int)dmfb->colors.size()-1 < dmfb->id_count[DMFB::NORMAL]){
        give_color(dmfb);
    }
    dmfb->id_count[DMFB::NORMAL]++;

    dmfb->table[x2][y2].size = dmfb->table[x1][y1].size/sqrt(2);
    dmfb->table[x3][y3].size = dmfb->table[x1][y1].size/sqrt(2);
    return 1;
}

bool MergeCommand::execute(DMFB *dmfb){
    if(x1>dmfb->col || x1<1 || y1>dmfb->row || y1<1){
        dmfb->Error(DMFB::OUT_OF_CHIP);
        return 0;
    }
    if(x2>dmfb->col || x2<1 || y2>dmfb->row || y2<1){
        dmfb->Error(DMFB::OUT_OF_CHIP);
        return 0;
    }
    if(x3>dmfb->col || x3<1 || y3>dmfb->row || y3<1){
        dmfb->Error(DMFB::OUT_OF_CHIP);
        return 0;
    }

    if(dmfb->table[x2][y2].type != DMFB::NORMAL || dmfb->table[x3][y3].type != DMFB::NORMAL){
        dmfb->Error(DMFB::OUT_OF_CHIP);
        return 0;
    }

    dmfb->table[x1][y1].id = dmfb->id_count[DMFB::NORMAL];
    if((int)dmfb->colors.size()-1 < dmfb->id_count[DMFB::NORMAL]){
        give_color(dmfb);
    }
    dmfb->id_count[DMFB::NORMAL]++;

    if(x2 != x1){
        dmfb->table[x1][y1].type = DMFB::MERGE_H;
        dmfb->table[x2][y2].type = DMFB::MERGE;
        dmfb->table[x3][y3].type = DMFB::MERGE;
    }
    if(y2 != y1){
        dmfb->table[x1][y1].type = DMFB::MERGE_V;
        dmfb->table[x2][y2].type = DMFB::MERGE;
        dmfb->table[x3][y3].type = DMFB::MERGE;
    }

    return 1;
}


bool MergeCommandFinish::execute(DMFB *dmfb){
    dmfb->table[x2][y2].type = DMFB::TRACE;
    dmfb->table[x2][y2].trace.push_back(dmfb->table[x2][y2].id);

    dmfb->table[x3][y3].type = DMFB::TRACE;
    dmfb->table[x3][y3].trace.push_back(dmfb->table[x3][y3].id);

    if(dmfb->trace_exist[x2][y2].size()<dmfb->table[x2][y2].trace.size())
        dmfb->trace_exist[x2][y2].push_back(1);
    if(dmfb->trace_exist[x3][y3].size()<dmfb->table[x3][y3].trace.size())
        dmfb->trace_exist[x3][y3].push_back(1);

    dmfb->table[x1][y1].type = DMFB::NORMAL;
    dmfb->table[x1][y1].size = sqrt(dmfb->table[x2][y2].size*dmfb->table[x2][y2].size +
                                    dmfb->table[x3][y3].size*dmfb->table[x3][y3].size);
    return 1;
}

bool OutputCommand::execute(DMFB *dmfb){
    if(x1>dmfb->col || x1<1 || y1>dmfb->row || y1<1){
        dmfb->Error(DMFB::OUT_OF_CHIP);
        return 0;
    }

    if(dmfb->table[x1][y1].type != DMFB::NORMAL){
        dmfb->Error(DMFB::OUT_OF_CHIP);
        return 0;
    }
    dmfb->table[x1][y1].type = DMFB::OUTPUT_DROP;

    dmfb->table[x1][y1].trace.push_back(dmfb->table[x1][y1].id);
    if(dmfb->trace_exist[x1][y1].size()<dmfb->table[x1][y1].trace.size())
        dmfb->trace_exist[x1][y1].push_back(1);

    return 1;
}
