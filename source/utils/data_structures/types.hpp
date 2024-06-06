#pragma once
#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <string>
#include <iostream>
#include "position.hpp"
#include <map>

// Mutavol namespace
namespace mtv {
    enum class TokenType {
        IDENTIFIER,
        OPERATOR,
        DELIMITER,
        KEYWORD,
        DTYPE,
        LITERAL,
        UNIDENTIFIED
    };

    struct Token_t {
        std::wstring lexem;
        TokenType type;
        Position pos;
    };

    typedef std::map<std::wstring, std::wstring> Transition;
    typedef std::map<std::wstring, Transition> State;
} // namespace mtv

#endif // STRUCTS_HPP
