#pragma once
#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <string>

enum class TokenType
{
    IDENTIFIER,
    OPERATOR,
    DELIMITER,
    KEYWORD,
    LITERAL,
    UNIDENTIFIED
};

struct Token_t
{
    std::string lexem;
    TokenType type;
};

#endif // STRUCTS_HPP