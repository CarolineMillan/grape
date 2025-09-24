#pragma once

#include <unordered_set>
#include <vector>

#include "nfa_fragment.h"
#include "parser.h"
#include "state.h"

using std::vector, std::unique_ptr, std::unordered_set;

class NFA {
public:
    // default constructor
    NFA() = default;
    // construct from values
    NFA(State* start, State* accept, vector<unique_ptr<State>> states);
    // default deconstructor
    ~NFA() = default;

    NFA(const NFA&) = delete;
    NFA& operator=(const NFA&) = delete;
    NFA(NFA&&) = default;
    NFA& operator=(NFA&&) = default;


    // add a state to states and return the raw pointer
    State* new_state(bool);

    // add final fragment to nfa that sets start and accept states
    void add_final_fragment(NFAFragment final);

    // run the NFA with an input string
    bool run(std::string const&);
private:
    // NFA internals (each transition is stored in a state)
    State* start = nullptr;
    // use a unique_ptr so that the addresses are stable as you add to the vector
    vector<unique_ptr<State>> states;
    State* accept = nullptr;

    // helpers
    void epsilon_closures(unordered_set<State*>& start_states);
    unordered_set<State*> dfs(State* start_vertex);
    void dfsr(unordered_set<State*>& visited, State* current_state);
};