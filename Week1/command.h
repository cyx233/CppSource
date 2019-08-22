#ifndef COMMAND_H
#define COMMAND_H
#include <vector>
using std::vector;

class DMFB;

class Command
{
public:
    Command();
    virtual void execute(DMFB* dmfb);
    virtual ~Command();
};

class InputCommand: public Command{
public:
    InputCommand(int x, int y):x1(x),y1(y){}
    void execute(DMFB* dmfb);
    int x1,y1;
};

class MoveCommand: public Command{
public:
    MoveCommand(int a1,int b1,int a2,int b2):x1(a1),y1(b1),x2(a2),y2(b2){}
    void execute(DMFB* dmfb);
    int x1,y1,x2,y2;
};

class SplitCommand: public Command{
public:
    SplitCommand(int a1, int b1, int a2, int b2, int a3, int b3):
                x1(a1),y1(b1),x2(a2),y2(b2),x3(a3),y3(b3){}
    void execute(DMFB* dmfb);
    int x1,y1,x2,y2,x3,y3;
};

class SplitCommandFinish: public SplitCommand{
public:
    SplitCommandFinish(int x1,int y1,int x2,int y2,int x3,int y3):SplitCommand (x1,y1,x2,y2,x3,y3){}
    void execute(DMFB* dmfb);
};

class MergeCommand: public Command{
public:
    MergeCommand(int a1, int b1, int a2, int b2, int a3, int b3):
                x1(a1),y1(b1),x2(a2),y2(b2),x3(a3),y3(b3){}
    void execute(DMFB* dmfb);
    int x1,y1,x2,y2,x3,y3;
};

class MergeCommandFinish: public MergeCommand{
public:
    MergeCommandFinish(int x1,int y1,int x2,int y2,int x3,int y3):MergeCommand (x1,y1,x2,y2,x3,y3){}
    void execute(DMFB* dmfb);
};



class OutputCommand: public Command{
public:
    OutputCommand(int a1,int b1):x1(a1),y1(b1){}
    void execute(DMFB* dmfb);
    int x1,y1;
};

#endif // COMMAND_H
