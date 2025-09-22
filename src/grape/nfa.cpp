#include "nfa.h"
#include "nfa_fragment.h"
#include "parser.h"

NFA::NFA(State* start, State* accept, vector<unique_ptr<State>> states_vec): start(start), accept(accept), states(std::move(states_vec)) {}

State* NFA::new_state(bool is_accept) {
    states.emplace_back(std::make_unique<State>(is_accept));
    return states.back().get();
}
void NFA::add_final_fragment(NFAFragment* final) {
    start = final->start;
    accept = final->accept;
}

bool NFA::run(std::string input_string) {
    return false;
}