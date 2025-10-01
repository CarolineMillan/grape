//
// Created by Caroline Millan on 16/09/2025.
//

#pragma once
#include "state.h"

struct NFAFragment {
    State* start;
    State* accept;

public:
    NFAFragment(State* start, State* accept): start(start), accept(accept) {}
    ~NFAFragment()=default;
    NFAFragment()=default;
};
