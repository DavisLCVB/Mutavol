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
        OPERATORDOBLE,
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
    
    struct ResultAPD {
        std::string nextState;
        std::wstring actionP1;
        std::wstring actionP2;
    };

    using P2Result = std::map<std::wstring, ResultAPD>;
    using P1Result = std::map<std::wstring, P2Result>;
    using Simbol = std::map<std::wstring, P1Result>;
    using APDState = std::map<std::string, Simbol>;
} // namespace mtv

#endif // STRUCTS_HPP
