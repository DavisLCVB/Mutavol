#include "args.hpp"

#include <iostream>
#include <regex>
#include <filesystem>
#include "../utils/utils.hpp"
#include <string>

namespace mtv {
    LinkedList<std::string> *Args::args_template =
            ListFactory::create_linked_list<std::string>();
    LinkedList<std::string> *Args::args = ListFactory::create_linked_list<std::string>();
    std::string Args::input_file{};
    std::string Args::output_file{};

    Args::ArgsResult Args::process_args(const int argc, char *argv[]) {
        initialize_templates();
        if (!check_number_of_args(argc)) {
            return {false, true};
        }
        convert_args_to_list(argc, argv);
        return analize_list();
    }

    void Args::initialize_templates() {
        args_template->push("--help");
        args_template->push("--version");
        args_template->push("--input");
        args_template->push("--output");
        args_template->push("-h");
        args_template->push("-v");
        args_template->push("-i");
        args_template->push("-o");
    }

    bool Args::check_number_of_args(const int argc) {
        return argc >= 2;
    }

    void Args::convert_args_to_list(const int argc, char *argv[]) {
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            args->push(arg);
        }
    }

    Args::ArgsResult Args::analize_list() {
        auto it = args->begin();
        auto d = args;
        while (it != args->end()) {
            if (const std::string arg = *it; args_template->contains(arg)) {
                if (arg == "--help" || arg == "-h") {
                    process_help();
                    return {true, true};
                }
                if (arg == "--version" || arg == "-v") {
                    process_version();
                    return {true, true};
                }
                if (arg == "--input" || arg == "-i") {
                    if (++it == args->end()) {
                        return bad_exit_process("Missing input file");
                    }
                    process_input(*it);
                } else if (arg == "--output" || arg == "-o") {
                    if (++it == args->end()) {
                        return bad_exit_process("Missing output file");
                    }
                    process_output(*it);
                }
            } else {
                return bad_exit_process("Invalid argument: " + arg);
            }
            ++it;
        }
        if (input_file.empty()) {
            return bad_exit_process("Missing input file");
        }
        return {true, false, input_file, output_file};
    }

    void Args::process_help() {
        std::cout << config::project << " " << config::version << '\n';
        std::cout << "Usage: mtv [options]\n";
        std::cout << "Options:\n";
        std::cout << "  -h, --help\t\tShow this help message and exit\n";
        std::cout << "  -v, --version\t\tShow version information and exit\n";
        std::cout << "  -i <file>, --input <file> \t\tInput file\n";
        std::cout << "  -o <file>, --output <file> \t\tOutput file\n";
    }

    void Args::process_version() {
        std::cout << config::project << " " << config::version << '\n';
    }

    void Args::process_input(const std::string &input_param) {
        process_file(input_file, input_param);
        // std::cout << "Input file: " << input_file << '\n';
    }

    void Args::process_output(const std::string &output_param) {
        process_file(output_file, output_param);
        // std::cout << "Output file: " << output_file << '\n';
    }

    Args::ArgsResult Args::bad_exit_process(const std::string &message) {
        std::cerr << message << '\n';
        return {false, true};
    }

    void Args::process_file(std::string &file, const std::string &param) {
        file = param;
#ifdef WIN32
        win_to_linux_fmt(file);
#endif // WIN32
        if (is_full_path(file)) {
            return;
        }
        preprocess_file(file);
    }

    void Args::preprocess_file(std::string &file) {
        auto current_path = std::filesystem::current_path();
        while (file.find("../") != std::string::npos) {
            file = file.substr(3);
            current_path = current_path.parent_path();
        }
        if (file.find("./") != std::string::npos)
            file = file.substr(2);
        file = current_path.string() + "/" + file;
#ifdef WIN32
        win_to_linux_fmt(file);
#endif // WIN32
    }

    bool Args::is_full_path(const std::string &file) {
#ifdef  WIN32
        const std::regex pattern(R"([A-Z]:(\/\w+)+(\.\w+)+)");
        return std::regex_match(file, pattern);
#endif // WIN32
#ifdef __linux__
        const std::regex pattern(R"(\/\w+(\/\w+)+(\.\w+)*)");
        return std::regex_match(file, pattern);
#endif // __linux__
    }

    void Args::win_to_linux_fmt(std::string &path) {
        for (auto &c: path) {
            if (c == '\\') {
                c = '/';
            }
        }
    }
} // namespace mtv
