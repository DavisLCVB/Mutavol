#pragma once
#ifndef Classifier_HPP
#define Classifier_HPP

#include "../types.hpp"
#include "config.hpp"
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
#include <regex>

#if DEBUG
#include <iostream>
#endif // DEBUG

namespace mia_kalinux
{
    class Classifier final
    {
    private:
        static std::unique_ptr<Classifier> instance;
        static std::once_flag init_instance_flag;

        Token_t tok;

        static void init_instance();
        Classifier() = default;

    public:
        // Cadena de prueba que sustituye temporalmente a slicer
        int test_string_index = 0;
        static const std::vector<std::string> TEST_STRINGS;

        static const std::string IDENTIFIERS;
        static const std::string OPERATORS;
        static const std::string DELIMITERS;
        static const std::string KEYWORDS;
        static const std::string LITERALS;

        ~Classifier() = default;
        Classifier(const Classifier &) = delete;
        Classifier &operator=(const Classifier &) = delete;

        static Classifier &get_instance();

        void classify();

        const Token_t next_token();

        bool is_identifier();

        bool is_operator();

        bool is_keyword();

        bool is_literal();

        bool is_delimiter();

    };

} // namespace Classifier
#endif // Classifier_HPP