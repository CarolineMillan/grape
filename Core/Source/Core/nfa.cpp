#include "nfa.h"

#include <iostream>
#include <ostream>

#include "nfa_fragment.h"

NFA::NFA(State* start, State* accept, vector<unique_ptr<State>> states_vec): start(start), accept(accept), states(std::move(states_vec)) {}

State* NFA::new_state(bool is_accept) {
    states.emplace_back(std::make_unique<State>(is_accept));
    return states.back().get();
}
void NFA::add_final_fragment(NFAFragment final) {
    start = final.start;
    accept = final.accept;
}

bool NFA::run(std::string const& input_string) {
    // initialise current_states by getting all the states reachable by epsilon alone from the nfa's start state
    // the nfa is basically a directed graph, so this is traversing a graph and adding the accept states to current_states
    // this can be a helper function, find epsilon closures from a given start state (or set of start states?), returns all states reachable by epsilons alone
    unordered_set<State*> current_states = {start};
    epsilon_closures(current_states);
    //std::cout << "c: " << current_states.size() << std::endl;

    for (const char ch : input_string) {
        //std::cout << ch << std::endl;
        unordered_set<State*> next_candidates;
        for (State* state : current_states) {
            // I want to add to next candidates every time, so initialise it outside of this for loop, then extend it inside this for loop, then get next_states at the end of this for loop (ie next states at the end of the loop for this character)
            unordered_set<State*> targets = state->get_targets(ch);
            //std::cout << "targets: " << targets.size() << std::endl;
            next_candidates.insert(targets.begin(), targets.end());
        }
        // do we rewrite current_states here or do we want to insert next_candidates into it?
        epsilon_closures(next_candidates);
        current_states = std::move(next_candidates);
        //current_states.insert(next_candidates.begin(), next_candidates.end());
    }

    // if nfa.accept is in current_states, return true, else false
    //std::cout << "c2: " << current_states.size() << std::endl;
    if (current_states.find(accept) != current_states.end()) {return true;}
    return false;
}

void NFA::dfsr(unordered_set<State*>& visited, State* current_state) {
    visited.insert(current_state);
    // get the 'neighbours' ie the states reachable by epsilon
    vector<State*> neighbours;
    for (Transition transition: current_state->transitions) {
        if (!transition.symbol) {
            //std::cout << "epsilon" << std::endl;
            // this is getting a list of neighbours
            neighbours.emplace_back(transition.target);
        }
    }

    for (State* state : neighbours) {
        if (visited.find(state) == visited.end()) {
            dfsr(visited, state);
        }
    }
}

unordered_set<State*> NFA::dfs(State* start_vertex) {
    unordered_set<State*> visited;
    dfsr(visited, start_vertex);
    return visited;
}

void NFA::epsilon_closures(unordered_set<State*>& start_states) {
    // adds epsilon closures to start_states
    for (State* state : start_states) {
        unordered_set<State*> closure = dfs(state);
        start_states.insert(closure.begin(), closure.end());
    }
}
