#pragma once
#ifndef PASER_HPP
#define PARSER_HPP
#include <memory>

#include "../../utils/data_structures/types.hpp"

namespace mtv
{
    class Parser
    {
        State afdVars;

    public:
        ~Parser() = default;

        Parser(const Parser &) = delete;

        Parser &operator=(const Parser &) = delete;

        static Parser &get_instance();

        static void init_instance();

        static void parse();

    private:
        static std::unique_ptr<Parser> instance;
        Parser();
    };
} // namespace mtv

#endif // PARSER_HPP
