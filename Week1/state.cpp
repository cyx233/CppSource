#include "state.h"

State::State(DMFB* a):dfmb(a){}
State::~State(){}

void StartState::next(){
    dfmb->set_state(new SetInputState(dfmb));
}

void SetInputState::next(){
    dfmb->set_state(new SetOutputState(dfmb));
}

void SetOutputState::next(){
    dfmb->set_state(new MainState(dfmb));
    dfmb->copy = dfmb->table;
}

void MainState::next(){
    dfmb->set_state(new StartState(dfmb));
}
