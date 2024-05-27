#pragma once
#ifndef Classifier_HPP
#define Classifier_HPP

#include "../../utilities/data_structures/types.hpp"
#include "config.hpp"
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
#include <regex>

#if DEBUG
#include <iostream>
#endif // DEBUG

namespace mtv {
    class Classifier {
    public:

        static const std::wstring IDENTIFIERS;
        static const std::wstring OPERATORS;
        static const std::wstring DELIMITERS;
        static const std::wstring KEYWORDS;
        static const std::wstring LITERALS;

        ~Classifier() = default;

        Classifier(const Classifier &) = delete;

        Classifier &operator=(const Classifier &) = delete;

        static Classifier &get_instance();

        static void init_slicer();

        void classify();

        Token_t next_token();

        bool is_identifier();

        bool is_operator();

        bool is_keyword();

        bool is_literal();

        bool is_delimiter();

    private:
        static std::unique_ptr<Classifier> instance;
        static std::once_flag init_instance_flag;

        Token_t tok;

        static void init_instance();

        Classifier() = default;
    };
} // namespace Classifier
#endif // Classifier_HPP
