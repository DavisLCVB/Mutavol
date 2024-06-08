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

    typedef std::map<std::wstring, std::wstring> Transition;
    typedef std::map<std::wstring, Transition> State;
    
    struct ResultAPD {
        std::string nextState;
        std::wstring actionP1;
        std::wstring actionP2;
    };

    typedef std::map<std::wstring, ResultAPD> P2Result;
    typedef std::map<std::wstring, P2Result> P1Result;
    typedef std::map<std::wstring, P1Result> Simbol;
    typedef std::map<std::string, Simbol> APDState;
} // namespace mtv

#endif // STRUCTS_HPP
