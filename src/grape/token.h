#pragma once

#include <array>

using std::array;

// Token is a POD-like type (Plain Old Data)
struct Token {
    // kind of token will be set via an enum for all possible tokens
    enum class KIND { Literal, CharClass, Star, Plus, Question, Concat, Alt, LParen, RParen, Dot};
    // set default to the literal character
    KIND kind = KIND::Literal;

    // payloads (some kinds need data, so this is where you store it)
    char ch = 0; // ch value for literals
    bool negate = false; // differentiates [abc] or [^abc]
    std::array<bool, 256> bitmap; // stores bitmap for character classes, for 256 ascii chars

    bool is_postfix_unary() const {
        switch (kind) {
            case KIND::Star:
            case KIND::Plus:
            case KIND::Question:
                return true;
            default:
                return false;
        }
    };

    bool is_operator() const {
        switch (kind) {
            case KIND::Star:
            case KIND::Plus:
            case KIND::Question:
            case KIND::Concat:
            case KIND::Alt:
                return true;
            default:
                return false;
        }
    };

    bool is_operand() const {
        switch(kind) {
            case KIND::Literal:
            case KIND::CharClass:
            case KIND::Dot:
                return true;
            default:
                return false;
        }
    };

    static constexpr int get_precedence(KIND kind) {
        switch (kind) {
            case KIND::Star:
            case KIND::Plus:
            case KIND::Question:
                return 3;
            case KIND::Concat:
                return 2;
            case KIND::Alt:
                return 1;
            default:
                return 0;
        }
    };

    void set_neg_char_class() {
        negate = true;
        bitmap.fill(true);
    }

    void add_to_char_class(char ascii) {
        // takes an ascii and adds it to the bitmap
        if (!negate) {bitmap[ascii] = true;}
        else {bitmap[ascii] = false;}

    };

    void add_range_to_char_class(char start, char end) {
        for (char ch = start; ch <= end; ch++) {
            add_to_char_class(ch);
        }
    };

    void clear_bitmap() {
        bitmap.fill(false);
    }
};