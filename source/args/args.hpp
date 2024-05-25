#pragma once
#ifndef ARGS_HPP
#define ARGS_HPP

/**
 * @file args.hpp
 * @author Davis Cartagena
 * @brief Class and methods to handle command line arguments
 */

// Include section

#include <string>
#include <utility>
#include "config.hpp"

#include "../utilities/data_structures/linked_list.hpp"

// Mutavol namespace
namespace mtv {
    /**
     * @class Args
     * @brief Class to handle command line arguments
     */
    class Args {
    public:
        /**
        * @struct ArgsResult
        * @brief Struct to store the result of the command line arguments process
        */
        struct ArgsResult;

        /**
         * @brief A method to process command line arguments
         * @param argc Quantity of arguments
         * @param argv Values of arguments
         * @return A struct with the result of the process
         */
        static ArgsResult process_args(int argc, char *argv[]);

    private:
        // Template of arguments
        static LinkedList<std::string> *args_template;
        // List of arguments
        static LinkedList<std::string> *args;
        // Input file
        static std::string input_file;
        // Output file
        static std::string output_file;

        /**
         * @brief Method to initialize the template of arguments
         */
        static void initialize_templates();

        /**
         * @brief Method to check the quantity of arguments
         * @param argc Quantity of arguments
         * @return true if the quantity of arguments is greater or equal to 2
         */
        static bool check_number_of_args(int argc);

        /**
         * @brief Method to convert the arguments (char**) to a LinkedList
         * @param argc Quantity of arguments
         * @param argv Values of arguments
         * @see LinkedList
         */
        static void convert_args_to_list(int argc, char *argv[]);

        /**
         * @brief Method to analyze the list of arguments
         * @return A struct with the result of the process
         */
        static ArgsResult analize_list();

        /**
         * @brief Method to process the help argument
         */
        static void process_help();

        /**
         * @brief Method to process the version argument
         */
        static void process_version();

        /**
         * @brief Method to process the input argument
         * @param input_param The input file
         */
        static void process_input(const std::string &input_param);

        /**
         * @brief Method to process the output argument
         * @param output_param The output file
         */
        static void process_output(const std::string &output_param);

        /**
         * @brief Method to exit in the case of a incorrect process of the arguments
         * @param message The message to show
         * @return A struct with the result of the process
         */
        static ArgsResult bad_exit_process(const std::string &message);

        /**
         * @brief Method to get a path of a file (can contain .\ or ..\)
         * @param file The file to process
         * @param param The parameter to process
         */
        static void process_file(std::string &file, const std::string &param);

        /**
         * @brief Method to get a full path of a file
         * @param file The file to preprocess
         */
        static void preprocess_file(std::string &file);

        /**
         * @brief Method to check if a file is a full path
         * @param file The file to check
         * @return true if the file is a full path
         */
        static bool is_full_path(const std::string &file);

        /**
         * @brief Method to convert a windows path to a linux path
         * @param path The path to convert
         */
        static void win_to_linux_fmt(std::string &path);
    };


    struct Args::ArgsResult {
        // true if the arguments were processed
        bool args_processed{false};
        // true if the program should exit
        bool should_exit{false};
        // input file
        std::string input{};
        // output file
        std::string output{};

        /**
         * @brief Constructor
         * @param args_processed If the arguments were processed
         * @param should_exit If the program should exit
         */
        ArgsResult(const bool args_processed,
                   const bool should_exit) : args_processed
                                             (args_processed),
                                             should_exit(
                                                 should_exit) {}

        /**
         * @brief Full constructor
         * @param args_processed If the arguments were processed
         * @param should_exit If the program should exit
         * @param input The input file
         * @param output The output file
         */
        ArgsResult(const bool args_processed, const bool should_exit,
                   std::string input,
                   std::string output) : args_processed(
                                             args_processed),
                                         should_exit(
                                             should_exit),
                                         input(std::move(input)),
                                         output(std::move(output)) {}

        ArgsResult() = default;
    };
} // namespace mtv

#endif // ARGS_HPP
