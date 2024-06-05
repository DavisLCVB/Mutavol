#pragma once
#ifndef READER_HPP
#define READER_HPP

/**
* @file reader.hpp
* @author Davis Cartagena
* @brief Class and methods to read the input file
*/

// Include section
#include <memory>
#include <string>
#include <utility>
#include "../scanner.hpp"

// Mutavol namespace
namespace mtv {
    // Using space
    using std::string;
    class Scanner::Reader {
    public:

        /**
        * @brief Gets the instance of the Reader class
        * @return The instance of the Reader class
        */
        static Reader &get_instance(const std::string &input_file);

        // Deleted copy constructor
        Reader(const Reader &) = delete;

        // Deleted copy assignment operator
        Reader &operator=(const Reader &) = delete;

        /**
        * @brief Method to read the input file
        * @return true if the file was read successfully, false otherwise
        */
        [[nodiscard]]
        bool read() const;

    private:
        // string to store the input file
        string input_file;
        // Pointer to the unique instance of the class
        static std::unique_ptr<Reader> instance;

        /**
        * @brief Method to verify if the file exists
        * @return true if the file exists, false otherwise
        */
        [[nodiscard]]
        bool verify() const;

        /**
        * @brief Method to read the input file
        * @return true if the file was read successfully, false otherwise
        */
        [[nodiscard]]
        bool read_file() const;

        /**
        * @brief Method to delete comments and empty lines of de buffer
        */
        static void clean();

        /**
        * @brief Method to delete line comments and block comments
        */
        static void remove_comments();

        /**
        * @brief Method to delete empty lines
        */
        static void remove_lines();

        /**
        * Constructor for the class
        */
        explicit Reader(string input_file) : input_file(std::move(input_file)) {}

        /**
        * Method to initialize the pointer to the unique instance of the class
        */
        static void init_instance(const std::string &input_file);
    }; // class Reader
} // namespace mtv

#endif // READER_HPP
