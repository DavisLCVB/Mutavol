#ifndef FORMAT_HPP
#define FORMAT_HPP

#include "../utils.hpp"
#include <iostream>
#include <string>

namespace mtv
{
    inline std::ostream &operator<<(std::ostream &os, const Position &pos)
    {
        os << pos.row << ";" << pos.column;
        return os;
    }

    inline std::wostream &operator<<(std::wostream &os, const Position &pos)
    {
        os << pos.row << ";" << pos.column;
        return os;
    }

    inline std::string type_str(const TokenType &type)
    {
        std::string return_s;
        switch (type)
        {
        case TokenType::IDENTIFIER:
            return_s = "IDENTIFIER";
            break;
        case TokenType::OPERATORCOMP:
            return_s = "OPERATORCOMP";
            break;
        case TokenType::OPERATOREXP:
            return_s = "OPERATOREXP";
            break;
        case TokenType::OPERATORDOBLE:
            return_s = "OPERATORDOBLE";
            break;
        case TokenType::DELIMITER:
            return_s = "DELIMITER";
            break;
        case TokenType::KEYWORD:
            return_s = "KEYWORD";
            break;
        case TokenType::DTYPE:
            return_s = "DTYPE";
            break;
        case TokenType::LITERAL:
            return_s = "LITERAL";
            break;
        case TokenType::UNIDENTIFIED:
            return_s = "UNIDENTIFIED";
            break;
        default:
            return_s = "UNKNOWN";
            break;
        }
        return return_s;
    }

    inline std::wstring type_wstr(const TokenType &type)
    {
        std::wstring return_ws;
        switch (type)
        {
        case TokenType::IDENTIFIER:
            return_ws = L"IDENTIFIER";
            break;
        case TokenType::OPERATOREXP:
            return_ws = L"OPERATOREXP";
            break;
        case TokenType::OPERATORCOMP:
            return_ws = L"OPERATORCOMP";
            break;
        case TokenType::OPERATORDOBLE:
            return_ws = L"OPERATORDOBLE";
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
        return return_ws;
    }

    inline std::ostream &operator<<(std::ostream &os, const TokenType &type)
    {
        const auto return_s = type_str(type);
        os << return_s;
        return os;
    }

    inline std::wostream &operator<<(std::wostream &os, const TokenType &type)
    {
        const auto return_ws = type_wstr(type);
        os << return_ws;
        return os;
    }

    inline std::wostream &operator<<(std::wostream &os, const Token_t &token)
    {
        os << (token.lexem == L"\n" ? L"\\n" : token.lexem) << "\t[" << token.type << "]"
           << "(" << token.pos << ")";
        return os;
    }
} // namespace mtv

#endif // FORMAT_HPP
