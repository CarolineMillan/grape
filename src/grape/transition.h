#pragma once
#include "state.h"

// forward declaration (interesting: you'd need a Box or something to do this in rust -- manually use heap allocation with pointers to avoid the infinite size type error that comes with self-referential structs)
struct State;

// an escape character that means this transition doesn't consume a char
const char EPSILON = '\0';

struct Transition {
    State* target;
    char symbol;

public:
    Transition(char symbol, State* target) {
        this->target = target;
        this->symbol = symbol;
    }
    Transition()=default;
    ~Transition()=default;
};
