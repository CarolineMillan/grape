//
// Created by Caroline Millan on 16/09/2025.
//
// parses a regex string into a vector of tokens in postfix form

#include <stack>
#include <string>

#include "regex_compiler.h"

#include <iostream>

using std::stack;
using std::string;

constexpr std::optional<char> EPSILON = std::nullopt;

vector<Token> RegexCompiler::parse(const string& pattern)
{
    tokenize(pattern);
    add_concats();
    to_postfix();

    return postfix_tokens;
}


/* -------------------- TOKENIZE ---------------------- */

void RegexCompiler::tokenize(const string &pattern) {
    // tokenizes the pattern string

    // make sure we're starting with an empty list of tokens
    tokens.clear();

    // TODO switch statement (maybe)

    // loop through the pattern
    for (int i = 0 ; i < pattern.size() ; i++) {
        char ch = pattern[i];

        if (ch == '\\') {
            // look at the next char that's being escaped, that will give you the token::KIND
            // maybe do this in a separate parse_escaped() method
            if (i+1 >= pattern.size()-1) {
                // then it's the last char and it's not escaping anything, so just add the literal
                Token t;
                t.ch = ch;
                t.kind = Token::KIND::Literal;
                tokens.push_back(t);
                continue;
            }
            else {
                parse_escaped(pattern[i+1]);
                // need to skip the escaped character, so add one to i
                i++;
                continue;
            }
        }
        else if (ch == '[') {
            if (i+1 >= pattern.size()-1) {
                // then it's the last char and it's not escaping anything, so just add the literal
                Token t;
                t.ch = ch;
                t.kind = Token::KIND::Literal;
                tokens.push_back(t);
                continue;
            }
            else {
                i = parse_char_class(pattern, i+1);
            }
        }
        else if (ch == '*') {
            Token t;
            t.kind = Token::KIND::Star;
            tokens.push_back(t);
        }

        else if (ch == '+') {
            Token t;
            t.kind = Token::KIND::Plus;
            tokens.push_back(t);
        }
        else if (ch == '?') {
            Token t;
            t.kind = Token::KIND::Question;
            tokens.push_back(t);
        }
        else if (ch == '|') {
            Token t;
            t.kind = Token::KIND::Alt;
            tokens.push_back(t);
        }
        else if (ch == '(') {
            Token t;
            t.kind = Token::KIND::LParen;
            tokens.push_back(t);
        }
        else if (ch == ')') {
            Token t;
            t.kind = Token::KIND::RParen;
            tokens.push_back(t);
        }
        else if (ch == '.') {
            parse_dot();
        }
        else {
            Token t;
            t.ch = ch;
            t.kind = Token::KIND::Literal;
            tokens.push_back(t);
        }
    }
};

void RegexCompiler::parse_escaped(const char ch) {
    // want to match ch to any of the regex things we support, else just save the literal
    // TODO add \s for whitespace
    Token t;
    switch (ch) {
        case 'd':
            // matches all digits ascii 48-57
            t.kind = Token::KIND::CharClass;
            t.add_range_to_char_class('0','9');
            break;
        case 'w':
            // matches all alphanumeric chars
            t.kind = Token::KIND::CharClass;
            t.add_range_to_char_class('0','9');
            t.add_range_to_char_class('a','z');
            t.add_range_to_char_class('A','Z');
            t.add_to_char_class('_');
            break;
        default:
            t.ch = ch;
            t.kind = Token::KIND::Literal;
            break;
    }
    tokens.push_back(t);
};

int RegexCompiler::parse_char_class(const string &pattern, int i) {
    // create a token for char class [] or [^] starting at pattern[i]
    // two passes -- first one to find the ], second one if you don't find it and need to use a literal instead
    // for now take everything in the character class as a literal character, no escapes or anything
    Token t;
    bool closed = false;
    int original_index = i;

    // check for a negative character class
    if (pattern[i] == '^') {
        t.set_neg_char_class();
    }

    // loop through pattern until you find the ]
    for (i; i < pattern.size(); i++) {
        if (pattern[i] == ']') {
            closed = true;
            t.kind = Token::KIND::CharClass;
            tokens.push_back(t);
            break;
        }
        else {
            t.add_to_char_class(pattern[i]);
        }
    }
    if (!closed) {
        // second pass to add a literal instead
        for (int j = original_index; j < pattern.size(); j++) {
            Token t_lit;
            t_lit.kind = Token::KIND::Literal;
            t_lit.ch = pattern[j];
            tokens.push_back(t_lit);
        }
    }
    return i;
};

void RegexCompiler::parse_dot() {
    Token t;
    t.kind = Token::KIND::CharClass;
    char before_newline = '\n'-1;
    char after_newline = '\n'+1;
    t.add_range_to_char_class(0, before_newline);
    t.add_range_to_char_class(after_newline, -1);
    tokens.push_back(t);
};

void RegexCompiler::add_concats() {
    // adds concats to tokens
    Token previous;
    bool first = true;
    for (Token current : tokens) {
        if (!first && should_concat(previous, current)) {
            Token t;
            t.kind = Token::KIND::Concat;
            concat_tokens.push_back(t);
        }
        concat_tokens.push_back(current);
        previous = current;
        first = false;
    }
};

bool RegexCompiler::should_concat(const Token& previous, const Token& current) {
    // may want to check edge cases in future, if you expand it
    return (previous.is_operand() || previous.kind == Token::KIND::RParen || (previous.kind == Token::KIND::Star || previous.kind == Token::KIND::Plus || previous.kind == Token::KIND::Question) )
    && (current.is_operand() || current.kind == Token::KIND::LParen);
}

/* --------------------- TO POSTFIX -------------------- */

void RegexCompiler::to_postfix() {
    // gets tokens into postfix format using shunting-yard algorithm

    //make sure we have a fresh postfix_tokens
    postfix_tokens.clear();

    // make a stack to store the operators on
    stack<Token> st;

    for (const Token& token : concat_tokens) {
        if (token.is_operand() || token.is_postfix_unary()) {
            postfix_tokens.push_back(token);
            continue;
        }
        else if (token.is_operator()) {
            while (!st.empty() && st.top().is_operator()) {
                // all your operators are left associative. If you add in right associative operators then you'll need a check here
                if (Token::get_precedence(st.top().kind) >= Token::get_precedence(token.kind)) {
                    postfix_tokens.push_back(st.top());
                    st.pop();
                }
                else break;
            }
            st.push(token);
        }
        else if (token.kind == Token::KIND::LParen) {
            st.push(token);
        }
        else if (token.kind == Token::KIND::RParen) {
            // pop everything until you reach an LParen, then discard both parens
            while (!st.empty() && st.top().kind != Token::KIND::LParen) {
                Token el = st.top();
                st.pop();
                postfix_tokens.push_back(el);
            }
            if (st.empty() || st.top().kind != Token::KIND::LParen) {
                // TODO error message, i think we may have confirmed that there is an LParen when we tokenized the input pattern
            }
            if (!st.empty()) st.pop();
        }
        //st.push(token);
    }

    // add the remaining tokens from the stack
    while (!st.empty()) {
        Token t = st.top();
        st.pop();
        if (t.kind != Token::KIND::LParen && t.kind != Token::KIND::RParen) postfix_tokens.push_back(t);
    }
};

/* --------------------- COMPILE TO NFA -------------------- */
NFA RegexCompiler::compile(vector<Token>& tokens) {
        // Thompson's construction
        stack<NFAFragment> fragments;
        //vector<unique_ptr<State>> states;

        // TODO if you can allocate space for the vector upfront then you don't need smart pointers
        NFA nfa = NFA();

        for (Token& token : tokens) {
                // rules go here
                switch (token.kind) {
                case Token::KIND::Literal:
                        {
                                // add start and accept states to nfa
                                State* start = nfa.new_state(false);
                                State* accept = nfa.new_state(true);

                                // add transition to start state
                                start->transitions.emplace_back(token.ch, accept);

                                // add fragment to fragments stack
                                fragments.emplace(start, accept);

                                break;
                        }
                case Token::KIND::CharClass:
                        {
                                // FIXME there's surely a more efficient way of doing this. Add another type to the transition struct for ranges?
                                // add start and accept states to nfa
                                State* start = nfa.new_state(false);
                                State* accept = nfa.new_state(true);

                                // add transition to start state for each char
                                for (int c = 0; c < 256; c++) {
                                        if (token.bitmap[c]) {
                                                start->transitions.emplace_back(static_cast<unsigned char>(c), accept);
                                        }
                                }

                                // add fragment to fragments stack
                                fragments.emplace(start, accept);

                                break;
                        }
                case Token::KIND::Concat:
                        {
                                // our to_postfix means that we're in postfix notation, so the operator comes AFTER both operands
                                // so pop two fragments off the stack for our operands

                                // TODO should there be an error if stack is empty?
                                if (fragments.size() < 2) throw std::logic_error("Malformed postfix expression: concat needs 2 operands");

                                NFAFragment b = fragments.top();
                                fragments.pop();
                                NFAFragment a = fragments.top();
                                fragments.pop();

                                // now we concat them to get a new fragment

                                // we want an epsilon transition between a's accept state and b's start state
                                a.accept->transitions.emplace_back(EPSILON, b.start);

                                // add the new fragment to fragments
                                fragments.emplace(a.start, b.accept);

                                break;
                        }
                case Token::KIND::Alt:
                        {
                                // A|B accepts when there is either A or B
                                // TODO should there be an error if stack is empty?
                                if (fragments.size() < 2) throw std::logic_error("Malformed postfix expression: alt needs 2 operands");

                                // add start and accept states to nfa
                                State* start = nfa.new_state(false);
                                State* accept = nfa.new_state(true);

                                NFAFragment b = fragments.top();
                                fragments.pop();
                                NFAFragment a = fragments.top();
                                fragments.pop();

                                // add epsilon transitions from start to a.start and b.start
                                start->transitions.emplace_back(EPSILON, a.start);
                                start->transitions.emplace_back(EPSILON, b.start);

                                // add epsilon transitions from a.accept and b.accept to accept
                                a.accept->transitions.emplace_back(EPSILON, accept);
                                b.accept->transitions.emplace_back(EPSILON, accept);

                                // add fragment to fragments stack
                                fragments.emplace(start, accept);

                                break;
                        }
                case Token::KIND::Star:
                        {
                                // A* accepts when there are 0 or more A's
                                // TODO should there be an error if stack is empty?
                                if (fragments.empty()) throw std::logic_error("Malformed postfix expression: star needs an operand");

                                // add start and accept states to nfa
                                State* start = nfa.new_state(false);
                                State* accept = nfa.new_state(true);

                                NFAFragment a = fragments.top();
                                fragments.pop();

                                // add epsilon transitions from start to a.start and accept (accepts 0)
                                start->transitions.emplace_back(EPSILON, a.start);
                                start->transitions.emplace_back(EPSILON, accept);

                                // add epsilon transitions from a.accept to a.start and accept (accepts 1+)
                                a.accept->transitions.emplace_back(EPSILON, a.start);
                                a.accept->transitions.emplace_back(EPSILON, accept);

                                // add fragment to fragments stack
                                fragments.emplace(start, accept);

                                break;
                        }
                case Token::KIND::Question:
                        {
                                // A? is equiv to (nothing)|A
                                // TODO should there be an error if stack is empty?
                                if (fragments.empty()) throw std::logic_error("Malformed postfix expression: question needs an operand");

                                // add start and accept states to nfa
                                State* start = nfa.new_state(false);
                                State* accept = nfa.new_state(true);

                                NFAFragment a = fragments.top();
                                fragments.pop();

                                // add epsilon transitions from start to a.start and accept (accepts 0)
                                start->transitions.emplace_back(EPSILON, a.start);
                                start->transitions.emplace_back(EPSILON, accept);

                                // add epsilon transitions from a.accept to accept (accepts 1)
                                a.accept->transitions.emplace_back(EPSILON, accept);

                                // add fragment to fragments stack
                                fragments.emplace(start, accept);

                                break;
                        }
                case Token::KIND::Plus:
                        {
                                // TODO should there be an error if stack is empty?
                                if (fragments.empty()) throw std::logic_error("Malformed postfix expression: plus needs an operand");

                                // add start and accept states to nfa
                                State* start = nfa.new_state(false);
                                State* accept = nfa.new_state(true);

                                NFAFragment a = fragments.top();
                                fragments.pop();

                                // add epsilon transitions from start to a.start and accept (accepts a.start only)
                                start->transitions.emplace_back(EPSILON, a.start);

                                // add epsilon transitions from a.accept to a.start and accept (accepts 1+)
                                a.accept->transitions.emplace_back(EPSILON, a.start);
                                a.accept->transitions.emplace_back(EPSILON, accept);

                                // add fragment to fragments stack
                                fragments.emplace(start, accept);

                                break;
                        }
                default: { break; }
                }
        }

        // final fragment on stack will be the start and accept states for the NFA
        if (fragments.size() == 1) {
                NFAFragment final = fragments.top();
                nfa.add_final_fragment(final);
                return nfa;
        }
        else {
                throw std::logic_error("Malformed NFA: no final fragment for start and accept states");
        }
}
