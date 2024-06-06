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

    inline std::wstring type_str(const TokenType &type) {
        const auto green = L"\x1b[1;32m";
        const auto reset = L"\x1b[0m";
        auto return_ws = std::wstring();
        switch (type) {
            case TokenType::IDENTIFIER:
                return_ws = L"IDENTIFIER";
                break;
            case TokenType::OPERATOR:
                return_ws = L"OPERATOR";
                break;
            case TokenType::DELIMITER:
                return_ws = L"DELIMITER";
                break;
            case TokenType::KEYWORD:
                return_ws = L"KEYWORD";
                break;
            case TokenType::DTYPE:
                return_ws = L"DTYPE";
                break;
            case TokenType::LITERAL:
                return_ws = L"LITERAL";
                break;
            case TokenType::UNIDENTIFIED:
                return_ws = L"UNIDENTIFIED";
                break;
            default:
                return_ws = L"UNKNOWN";
                break;
        }
        return green + return_ws + reset;
    }
} // namespace mtv

#endif // STRUCTS_HPP
