#pragma once
#ifndef PASER_HPP
#define PARSER_HPP
#include <memory>
#include "../source/utils/format/format.hpp"
#include "../../utils/data_structures/types.hpp"
#include "../scanner/scanner.hpp"

namespace mtv
{
    class Parser
    {
        State afdVars;
        State afdFuncs;
        State afdCalls;
        State afdFor;
        bool error;
        Token_t current_token;
        std::string p_state;
        void X();
        void S();
        void H();
        void Y();

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
        bool evaluate_conditional();
    };
} // namespace mtv

#endif // PARSER_HPP
