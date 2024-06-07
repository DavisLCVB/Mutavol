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
        OPERATORCOMP,
        OPERATOREXP,
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

    using Transition = std::map<std::wstring, std::wstring>;
    using State = std::map<std::wstring, Transition>;
} // namespace mtv

#endif // STRUCTS_HPP
