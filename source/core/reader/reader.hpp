#pragma once
#ifndef READER_HPP
#define READER_HPP

#include <memory>
#include <string>
#include <utility>
#include <ranges>

namespace mtv {
    // Using space
    using std::string;

    // Using space
    class Reader {
    public:

        static Reader &get_instance(const std::string &input_file);

        Reader(const Reader &) = delete;

        Reader &operator=(const Reader &) = delete;

        [[nodiscard]]
        bool read() const;

    private:
        string input_file;
        static std::unique_ptr<Reader> instance;

        [[nodiscard]]
        bool verify() const;

        [[nodiscard]]
        bool read_file() const;

        static void clean();

        static void remove_comments();

        static void remove_lines();

        explicit Reader(string input_file) : input_file(std::move(input_file)) {}

        static void init_instance(const std::string &input_file);
    }; // class Reader
} // namespace mtv

#endif // READER_HPP
