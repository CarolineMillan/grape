//
// Created by Caroline Millan on 16/09/2025.
//
// compiles a vector of postfix tokens into an NFA using Thompson's NFA construction algorithm. Not sure that this needs to be a class, it is just one function
#pragma once
#include <vector>

#include "nfa.h"
#include "token.h"

class Compiler {
public:
    Compiler() = default;
    ~Compiler() = default;
    NFA compile(vector<Token>& tokens);
};
