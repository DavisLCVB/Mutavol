#pragma once
#ifndef PASER_HPP
#define PARSER_HPP
#include <memory>
#include "../source/utils/format/format.hpp"
#include "../../utils/data_structures/types.hpp"
#include "../scanner/scanner.hpp"
#include <stack>

namespace mtv
{
    class Parser
    {
        State afdVars;
        State afdFuncs;
        State afdCalls;
        State afdFor;
        
        APDState apd;
        std::stack<std::wstring> stack_func;
        std::stack<std::wstring> stack_bloq;

        bool error;
        Token_t current_token;
        std::string p_state;

    public:
        ~Parser() = default;

        Parser(const Parser &) = delete;

        Parser &operator=(const Parser &) = delete;

        static Parser &get_instance();

        static void init_parser();

        void parse();

    private:
        static std::unique_ptr<Parser> instance;

        Parser();

        void evaluate_whit_afd(const State &afd);

        void evaluate_apd();

        std::wstring evaluate_pila(std::stack<std::wstring> &pila);

        void pila_action(std::stack<std::wstring> &pila, std::wstring action);

        bool sgetValues(auto &putIn, auto &map, std::string key);

        bool wsgetValues(auto &putIn, auto &map, std::wstring key);
    };
} // namespace mtv

#endif // PARSER_HPP
