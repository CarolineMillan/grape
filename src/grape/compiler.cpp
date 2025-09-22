//
// Created by Caroline Millan on 16/09/2025.
//

#include "compiler.h"

#include <iostream>
#include <ostream>

#include "nfa_fragment.h"

#include <stack>
#include <stdexcept>

using std::stack, std::unique_ptr;

NFA Compiler::compile(vector<Token>& tokens) {
        // Thompson's construction
        stack<NFAFragment> fragments;
        //vector<unique_ptr<State>> states;
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
                nfa.add_final_fragment(&final);
                return nfa;
        }
        else {
                throw std::logic_error("Malformed NFA: no final fragment for start and accept states");
        }
}
