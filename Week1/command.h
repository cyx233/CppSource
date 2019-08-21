#ifndef COMMAND_H
#define COMMAND_H
#include <vector>
using std::vector;

class DMFB;

class Command
{
public:
    Command();
    virtual void execute(DMFB* dmfb){};
    virtual ~Command();
};

class InputCommand: public Command{
public:
    InputCommand(){}
    void execute(DMFB* dmfb);
};

class MoveCommand: public Command{
public:
    MoveCommand(){}
    void execute(DMFB* dmfb);
};

class SplitCommand: public Command{
public:
    SplitCommand(){}
    void execute(DMFB* dmfb);
};

class MixCommand: public Command{
public:
    MixCommand(){}
    void execute(DMFB* dmfb);
};

class MergeCommand: public Command{
public:
    MergeCommand();
    void execute(DMFB* dmfb);
};

class OutputCommand: public Command{
public:
    OutputCommand();
    void execute(DMFB* dmfb);
};

#endif // COMMAND_H
