#pragma once
#include "transition.h"

struct State {
    std::vector<Transition> transitions;
    bool is_accept;
public:
    State(bool accept=false): is_accept(accept) {};
    State()=default;
    ~State()=default;
};