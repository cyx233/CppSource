#ifndef STATE_H
#define STATE_H

#include "dmfb.h"


class State
{
public:
    State(DMFB* a);
    enum{
        START = 0,
        INPUT,
        OUTPUT,
        MAIN
    };
    virtual void next()=0;
    virtual int getstate()=0;
    virtual ~State();
    DMFB* dfmb;
};


class StartState: public State{
public:
    StartState(DMFB* a):State(a){}
    ~StartState(){}
    void next();
    int getstate(){return START;}
};

class SetInputState: public State{
public:
    SetInputState(DMFB* a):State(a){}
    ~SetInputState(){}
    void next();
    int getstate(){return INPUT;}
};

class SetOutputState: public State{
public:
    SetOutputState(DMFB* a):State(a){}
    ~SetOutputState(){}
    void next();
    int getstate(){return OUTPUT;}
};


class MainState: public State{
public:
    MainState(DMFB* a):State(a){}
    ~MainState(){}
    void next();
    int getstate(){return MAIN;}
};


#endif // STATE_H
