#pragma once
#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "../types.hpp"
#include "../../args/templates.hpp"
#if DEBUG
#include <iostream>
#endif // DEBUG

namespace scanner
{
    class Scanner final
    {
    private:
        static std::unique_ptr<Scanner> instance;
        static std::once_flag init_instance_flag;

        std::string source_string;
        std::string::iterator current;
        std::string::iterator end;
        Token_t tok;
        
        static void init_instance(std::string &source_string);
        Scanner() = default;
        Scanner(std::string &source_string);

    public:
        static Scanner &get_instance();
        static Scanner &get_instance(std::string &source_string);

        static const std::string OPERATORS;

        void scan();

        const Token_t next_token();

        bool valid_char(char c);

        bool is_identifier();

        bool is_number();

        bool is_operator();

        void case_double_symbols(std::string &double_symbol_token, std::string &token);
    };

} // namespace scanner
#endif // SCANNER_HPP