#pragma once
#ifndef SCANNER_HPP
#define SCANNER_HPP
#include <memory>

#include "../../utils/data_structures/types.hpp"

namespace mtv {
    class Scanner {
    public:
        static void init_scanner(const std::string &input_file) noexcept(false);

        static Token_t get();

    private:
        Scanner() = default;

        /**
         * @class Classifier
         * @brief Class to classify the tokens
         */
        class Classifier;
        /**
         * @class Reader
         * @brief Class to read the input file
         */
        class Reader;
        /**
         * @class Slicer
         * @brief Class to slice the input file
         */
        class Slicer;
    };
} // namespace mtv

#endif //SCANNER_HPP
