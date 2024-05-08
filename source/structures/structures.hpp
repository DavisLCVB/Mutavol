#pragma once
#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <string>
#include <vector>

namespace mtv{
    enum class TokenType{
        IDENTIFIER,
        KEYWORD,
        LITERAL,
        OPERATOR,
        DELIMITER
    };

    struct Token{
        TokenType type;
        std::wstring value;
    };

    using vec_token_t = std::vector<Token>;
    using vec_wstring_t = std::vector<std::wstring>;

    const std::vector<std::wstring> keywords = {
        L"auto", L"break", L"case", L"char", L"const", L"continue", L"default",
        L"do", L"double", L"else", L"enum", L"extern", L"float", L"for", L"goto",
        L"if", L"int", L"long", L"register", L"return", L"short", L"signed",
        L"sizeof", L"static", L"struct", L"switch", L"typedef", L"union",
        L"unsigned", L"void", L"volatile", L"while"
    };
}

#endif // STRUCTURES_HPP