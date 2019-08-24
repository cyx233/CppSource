#include "state.h"

State::State(DMFB* a):dmfb(a){}
State::~State(){}

void StartState::next(){
    dmfb->set_state(new SetInputState(dmfb));
}

void SetInputState::next(){
    dmfb->set_state(new SetOutputState(dmfb));
}

void SetOutputState::next(){
    dmfb->set_state(new SetWashState(dmfb));
}

void SetWashState::next(){
    dmfb->set_state(new SetWasteState(dmfb));
}

void SetWasteState::next(){
    dmfb->set_state(new MainState(dmfb));
}

void MainState::next(){
    dmfb->set_state(new StartState(dmfb));
}
