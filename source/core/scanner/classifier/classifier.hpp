#pragma once
#ifndef Classifier_HPP
#define Classifier_HPP

#include "../../../utils/utils.hpp"
#include "../scanner.hpp"
#include "../slicer/slicer.hpp"
#include <memory>
#include <regex>

namespace mtv {
    class Scanner::Classifier {
    public:
        ~Classifier() = default;

        Classifier(const Classifier &) = delete;

        Classifier &operator=(const Classifier &) = delete;

        static Classifier &get_instance();

        static void init_slicer();

        void classify();

        Token_t next_token();

    private:
        static const std::wstring IDENTIFIERS;
        static const std::wstring OPERATORS;
        static const std::wstring DELIMITERS;
        static const std::wstring KEYWORDS;
        static const std::wstring LITERALS;
        static std::unique_ptr<Classifier> instance;
        Token_t tok;

        static void init_instance();

        Classifier() = default;

        bool is_identifier();

        bool is_operator();

        bool is_keyword();

        bool is_literal();

        bool is_delimiter();
    };
} // namespace mtv
#endif // Classifier_HPP
