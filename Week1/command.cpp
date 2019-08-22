#include "dmfb.h"
#include "command.h"

Command::Command(){}

Command::~Command(){}

void Command::execute(DMFB *dmfb){
    int temp = dmfb->row;
    dmfb->row = 0;
    dmfb->row = temp;
}

void InputCommand::execute(DMFB* dmfb){
    if(x1>dmfb->col || x1<1 || y1>dmfb->row || y1<1){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }

    if(dmfb->table[x1][y1].type != DMFB::NOTHING){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }
    dmfb->table[x1][y1].type = DMFB::INPUT_DROP;
    dmfb->table[x1][y1].id = dmfb->id_count[DMFB::NORMAL];
    dmfb->table[x1][y1].size = 5;
    dmfb->id_count[DMFB::NORMAL]++;
}

void MoveCommand::execute(DMFB *dmfb){
    if(x1>dmfb->col || x1<1 || y1>dmfb->row || y1<1){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }

    if(x2>dmfb->col || x2<1 || y2>dmfb->row || y2<1){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }

    if(dmfb->table[x2][y2].type != DMFB::NOTHING){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }

    dmfb->table[x1][y1].type = DMFB::TRACE;
    dmfb->table[x1][y1].id = dmfb->table[x1][y1].id;

    dmfb->table[x2][y2].type = DMFB::NORMAL;
    dmfb->table[x2][y2].id = dmfb->table[x1][y1].id;
    dmfb->table[x2][y2].size = dmfb->table[x1][y1].size;
}

void SplitCommand::execute(DMFB *dmfb){
    if(x1>dmfb->col || x1<1 || y1>dmfb->row || y1<1){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }
    if(x2>dmfb->col || x2<1 || y2>dmfb->row || y2<1){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }
    if(x3>dmfb->col || x3<1 || y3>dmfb->row || y3<1){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }

    if(dmfb->table[x1][y1].type != DMFB::NORMAL){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }

    dmfb->table[x1][y1].type = DMFB::MERGE;
    if(x2<x1){
        dmfb->table[x2][y2].type = DMFB::MERGE_LEFT;
        dmfb->table[x3][y3].type = DMFB::MERGE_RIGHT;
    }
    if(x2>x1){
        dmfb->table[x2][y2].type = DMFB::MERGE_RIGHT;
        dmfb->table[x3][y3].type = DMFB::MERGE_LEFT;
    }
    if(y2>y1){
        dmfb->table[x2][y2].type = DMFB::MERGE_UP;
        dmfb->table[x3][y3].type = DMFB::MERGE_DOWN;
    }
    if(y2<y1){
        dmfb->table[x2][y2].type = DMFB::MERGE_DOWN;
        dmfb->table[x3][y3].type = DMFB::MERGE_UP;
    }
    dmfb->table[x2][y2].id = dmfb->table[x1][y1].id;
    dmfb->table[x2][y2].size = dmfb->table[x1][y1].size;
    dmfb->table[x3][y3].id = dmfb->table[x1][y1].id;
    dmfb->table[x3][y3].size = dmfb->table[x1][y1].size;
}

void SplitCommandFinish::execute(DMFB *dmfb){
    if(x1>dmfb->col || x1<1 || y1>dmfb->row || y1<1){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }
    if(x2>dmfb->col || x2<1 || y2>dmfb->row || y2<1){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }
    if(x3>dmfb->col || x3<1 || y3>dmfb->row || y3<1){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }

    if(dmfb->table[x1][y1].type != DMFB::MERGE){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }

    dmfb->table[x1][y1].type = DMFB::TRACE;

    dmfb->table[x2][y2].type = DMFB::NORMAL;
    dmfb->table[x3][y3].type = DMFB::NORMAL;
    dmfb->table[x2][y2].id = dmfb->id_count[DMFB::NORMAL];
    dmfb->table[x3][y3].id = dmfb->id_count[DMFB::NORMAL];
    dmfb->id_count[DMFB::NORMAL]++;
    dmfb->table[x2][y2].size = dmfb->table[x1][y1].size-1;
    dmfb->table[x3][y3].size = dmfb->table[x1][y1].size-1;
}

void MergeCommand::execute(DMFB *dmfb){
    if(x1>dmfb->col || x1<1 || y1>dmfb->row || y1<1){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }
    if(x2>dmfb->col || x2<1 || y2>dmfb->row || y2<1){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }
    if(x3>dmfb->col || x3<1 || y3>dmfb->row || y3<1){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }

    if(dmfb->table[x2][y2].type != DMFB::NORMAL || dmfb->table[x3][y3].type != DMFB::NORMAL){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }

    dmfb->table[x1][y1].type = DMFB::MERGE;
    dmfb->table[x1][y1].id = dmfb->id_count[DMFB::NORMAL];
    dmfb->id_count[DMFB::NORMAL]++;

    if(x2<x1){
        dmfb->table[x2][y2].type = DMFB::MERGE_LEFT;
        dmfb->table[x3][y3].type = DMFB::MERGE_RIGHT;
    }
    if(x2>x1){
        dmfb->table[x2][y2].type = DMFB::MERGE_RIGHT;
        dmfb->table[x3][y3].type = DMFB::MERGE_LEFT;
    }
    if(y2>y1){
        dmfb->table[x2][y2].type = DMFB::MERGE_UP;
        dmfb->table[x3][y3].type = DMFB::MERGE_DOWN;
    }
    if(y2<y1){
        dmfb->table[x2][y2].type = DMFB::MERGE_DOWN;
        dmfb->table[x3][y3].type = DMFB::MERGE_UP;
    }

    dmfb->table[x2][y2].id = dmfb->table[x1][y1].id;
    dmfb->table[x2][y2].size = dmfb->table[x1][y1].size;
    dmfb->table[x3][y3].id = dmfb->table[x1][y1].id;
    dmfb->table[x3][y3].size = dmfb->table[x1][y1].size;
}


void MergeCommandFinish::execute(DMFB *dmfb){
    if(x1>dmfb->col || x1<1 || y1>dmfb->row || y1<1){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }
    if(x2>dmfb->col || x2<1 || y2>dmfb->row || y2<1){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }
    if(x3>dmfb->col || x3<1 || y3>dmfb->row || y3<1){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }

    if(dmfb->table[x1][y1].type != DMFB::MERGE){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }

    dmfb->table[x2][y2].type = DMFB::TRACE;
    dmfb->table[x3][y3].type = DMFB::TRACE;

    dmfb->table[x1][y1].type = DMFB::NORMAL;
    dmfb->table[x1][y1].id = dmfb->id_count[DMFB::NORMAL];
    dmfb->id_count[DMFB::NORMAL]++;
    dmfb->table[x1][y1].size = dmfb->table[x1][y1].size+1;
}

void OutputCommand::execute(DMFB *dmfb){
    if(x1>dmfb->col || x1<1 || y1>dmfb->row || y1<1){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }

    if(dmfb->table[x1][y1].type != DMFB::NORMAL){
        dmfb->Error(DMFB::COMMAND_ERROR);
        return;
    }
    dmfb->table[x1][y1].type = DMFB::OUTPUT_DROP;
}
