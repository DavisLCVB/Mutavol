#pragma once
#ifndef ARGS_HPP
#define ARGS_HPP

/**
 * @file args.hpp
 * @author Davis Cartagena
 * @brief Class and methods to handle command line arguments
 */

// Include section
#include "config.hpp"
#include "templates.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <mutex>

namespace mtv {
    /**
     * @class Args
     * @brief Class to handle command line arguments
     * @author Davis Cartagena
     */
    class Args final {
    public:
        /**
         * @brief Gets the instance of the Args class
         * @return The instance of the Args class
         */
        static Args &get_instance();

        /**
         * @brief Gets the instance of the Args class
         * @param argc Number of arguments
         * @param argv Array of arguments
         * @return The instace of the Args class, with the arguments set
         */
        static Args &get_instance(int argc, char *argv[]);

        /**
         * @brief Starts the process of reading and setting the arguments
         */
        void recon();

        /**
         * @brief Checks if the program should exit
         * @return True if the program should exit, false otherwise
         */
        [[nodiscard]]
        bool should_exit() const { return exit; }

        /**
         * @brief Gets a copy of the output file path
         * @return  A copy of the output file path
         */
        [[nodiscard]]
        std::string get_output_file() const { return output_file; }

        /**
         * @brief Gets a copy of the input file path
         * @return A copy of the input file path
         */
        [[nodiscard]]
        std::string get_input_file() const { return input_file; }

        /**
         * @brief Deleted copy constructor
         */
        Args(const Args &) = delete;

        /**
         * @brief Deleted copy assignment operator
         */
        Args &operator=(const Args &) = delete;

        /**
        * @brief Destructor
        */
        ~Args() = default;

    private:
        // Instace of the class, to be used as a singleton
        static std::unique_ptr<Args> instance;
        // Flag to ensure the instance is only created once
        static std::once_flag init_instance_flag;

        // Vector of arguments
        std::vector<std::string> args;
        // Input file path
        std::string input_file{};
        // Output file path
        std::string output_file{};
        // Index of the current argument
        int index{0};
        // Flag to determine if the program should exit
        bool exit{false};
        // Flag to determine if the input file has been set
        bool input_file_set{false};

        /**
         * @brief Method to initialize the instance of the class
         * @param argc Number of arguments
         * @param argv Array of arguments
         */
        static void init_instance(int argc, char *argv[]);

        /**
         * @brief Method to get the next argument
         * @return The next argument
         */
        std::string get();

        /**
         * @brief Process the argument that sets the output file
         * @param val Value of the argument
         */
        void process_output(const std::string &val);

        /**
         * @brief Process the argument that sets the input file
         * @param val Value of the argument
         */
        void process_input(const std::string &val);

        /**
         * @brief Writes the version of the program to the standard output,
         * sets the exit flag to true and returns
         */
        void process_version();

        /**
         * @brief Writes the help message to the standard output,
         * sets the exit flag to true and returns
         */
        void process_help();

        /**
         * @brief Gets the current path of the program
         * @return The current path of the program
         */
        static std::filesystem::path get_current_path();

        /**
         * @brief Default constructor
         */
        Args() = default;

        /**
         * @brief Constructor with arguments
         * @param argc Number of arguments
         * @param argv Array of arguments
         */
        Args(int argc, char *argv[]);
    };
} // namespace mtv

#endif // ARGS_HPP
