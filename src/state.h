#ifndef STATE_H
#define STATE_H

#include "dmfb.h"


class State
{
public:
    State(DMFB* a);
    enum{
        START_STATE = 0,
        INPUT_STATE,
        OUTPUT_STATE,
        WASH_STATE,
        WASTE_STATE,
        MAIN_STATE
    };
    virtual void next()=0;
    virtual int getstate()=0;
    virtual ~State();
    DMFB* dmfb;
};


class StartState: public State{
public:
    StartState(DMFB* a):State(a){}
    ~StartState(){}
    void next();
    int getstate(){return START_STATE;}
};

class SetInputState: public State{
public:
    SetInputState(DMFB* a):State(a){}
    ~SetInputState(){}
    void next();
    int getstate(){return INPUT_STATE;}
};

class SetOutputState: public State{
public:
    SetOutputState(DMFB* a):State(a){}
    ~SetOutputState(){}
    void next();
    int getstate(){return OUTPUT_STATE;}
};


class SetWashState: public State{
public:
    SetWashState(DMFB* a):State(a) {}
    ~SetWashState(){}
    void next();
    int getstate(){return WASH_STATE;}
};

class SetWasteState: public State{
public:
    SetWasteState(DMFB* a):State(a){}
    ~SetWasteState(){}
    void next();
    int getstate(){return WASTE_STATE;}
};

class MainState: public State{
public:
    MainState(DMFB* a):State(a){}
    ~MainState(){}
    void next();
    int getstate(){return MAIN_STATE;}
};


#endif // STATE_H
