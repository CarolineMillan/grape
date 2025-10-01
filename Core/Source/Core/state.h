#pragma once
#include <iostream>

#include "transition.h"

using std::vector;

// TODO you can probably remove is_accept, check first though
// TODO if you add an ID to State then you can use a bitmap or vector for current_states when running the NFA instead of a set, which is faster. unordered set fine for now.
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
            //if (transition.symbol) std::cout << "symbol: " << *transition.symbol << "ch: " << ch << "\n";
            if (transition.symbol == ch) {
                targets.insert(transition.target);
            }
        }
        return targets;
    }
};