#pragma once
#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <string>

// Mutavol namespace
namespace mtv {
    enum class TokenType {
        IDENTIFIER,
        OPERATOR,
        DELIMITER,
        KEYWORD,
        LITERAL,
        UNIDENTIFIED
    };

    struct Token_t {
        std::string lexem;
        TokenType type;
    };
} // namespace mtv

#endif // STRUCTS_HPP
