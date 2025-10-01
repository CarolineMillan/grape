//
// Created by Caroline Millan on 16/09/2025.
//

#pragma once

#include <vector>
#include "token.h"
#include "nfa.h"

using std::vector, std::string;

class RegexCompiler {
public:
    RegexCompiler() = default;
    ~RegexCompiler() = default;
    vector<Token> parse(const string& pattern);
    // compile to NFA
    NFA compile(vector<Token>& tokens);

private:
    vector<Token> tokens;
    vector<Token> concat_tokens;
    vector<Token> postfix_tokens;

    // helper methods
    void tokenize(const string& pattern);
    void parse_escaped(const char ch);
    int parse_char_class(const string& pattern, int i);
    void parse_dot();
    void add_concats();
    static bool should_concat(const Token& previous, const Token& current);

    // convert to postfix notation
    void to_postfix();

};
