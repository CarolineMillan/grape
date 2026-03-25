#pragma once
#include <iostream>
#include <unordered_set>
#include "transition.h"

using std::vector;

struct State {
    std::vector<Transition> transitions;
    bool is_accept;
public:
    State(bool accept=false): is_accept(accept) {};
    State()=default;
    ~State()=default;

    std::unordered_set<State*> get_targets(char ch) {
        std::unordered_set<State*> targets;
        for (const Transition& transition : transitions) {
            if (transition.symbol == ch) {
                targets.insert(transition.target);
            }
        }
        return targets;
    }
};
