#include "args.hpp"

namespace args {
Args::Args(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        args.push_back(argv[i]);
    }
}

void Args::recon() {
    if(args.empty()){
        exit = true;
        return;
    }
    auto arg = get();
    while (arg != "") {
#if DEBUG
        std::cout << "Processing argument: " << arg << std::endl;
#endif
        if (templates.count(arg) != 0) {
            if (arg == "--o") {
                process_output(get());
            } else if (arg == "--help") {
                process_help();
                return;
            } else if (arg == "--version") {
                process_version();
                return;
            } else if (arg == "--i") {
                if (input_file_set) {
                    exit = true;
                    std::cerr << "Invalid argument: " << get() << std::endl;
                    return;
                } else {
                    process_input(get());
                }
            }
        } else {
            if (!input_file_set) {
                process_input(arg);
                arg = get();
                continue;
            }
            exit = true;
            std::cerr << "Invalid argument: " << arg << std::endl;
            return;
        }
        arg = get();
    }
}

std::string Args::get() {
    if (index < args.size()) {
        auto &s = args[index];
        this->index++;
        return s;
    }
    return "";
}

void Args::process_output(const std::string &val) {
#if DEBUG
    std::cout << "Processing output: " << val << std::endl;
#endif
    if (output_file.empty()) {
        if (output_file.empty()) {
            output_file = get_current_path().string() + "/" + val;
        } else {
            exit = true;
            std::cerr << "Invalid argument: " << val << std::endl;
        }
        if (val == "") {
            exit = true;
            std::cerr << "Invalid argument: " << val << std::endl;
        }
        if (val.starts_with(".\\") || val.starts_with("./")) {
            output_file = val.substr(2);
            output_file = get_current_path().string() + "/" + output_file;
        }
#ifdef __linux__
        if (val.starts_with("\\") || val.starts_with("/")) {
            output_file = val;
        }
#endif
#ifdef _WIN32
        auto vol = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        if (val.find(":") != std::string::npos &&
            vol.find(val[0]) != std::string::npos) {
            output_file = val;
        }
#endif
        if (val.starts_with("..\\") || val.starts_with("../")) {
            auto path = get_current_path();
            auto _val = val;
            while (_val.starts_with("..\\") || _val.starts_with("../")) {
                path = path.parent_path();
                _val = _val.substr(3);
            }
            output_file = path.string() + "/" + _val;
        }
    } else {
        exit = true;
        std::cerr << "Invalid argument: " << val << std::endl;
    }
}

void Args::process_version() {
    std::cout << config::project << std::endl;
    std::cout << "Version: " << config::version << std::endl;
    exit = true;
}

void Args::process_help() {
    std::cout << "Usage: " << config::executable << " [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "\t--o <output file> :\tOutput file" << std::endl;
    std::cout << "\t--i <input file>  :\tInput file" << std::endl;
    std::cout << "\t--help \t\t  :\tShow this help" << std::endl;
    std::cout << "\t--version\t  :\tShow version" << std::endl;
    exit = true;
}

void Args::process_input(const std::string &val) {
#if DEBUG
    std::cout << "Processing input: " << val << std::endl;
#endif
    if (input_file.empty()) {
        input_file = get_current_path().string() + "/" + val;
    } else {
        exit = true;
        std::cerr << "Invalid argument: " << val << std::endl;
    }
    if (val == "") {
        exit = true;
        std::cerr << "Invalid argument: " << val << std::endl;
    }
    if (val.starts_with(".\\") || val.starts_with("./")) {
        input_file = val.substr(2);
    }
#ifdef __linux__
    if (val.starts_with("\\") || val.starts_with("/")) {
        input_file = val;
    }
#endif
#ifdef _WIN32
    auto vol = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (val.find(":") != std::string::npos &&
        vol.find(val[0]) != std::string::npos) {
        input_file = val;
    }
#endif
    if (val.starts_with("..\\") || val.starts_with("../")) {
        auto path = get_current_path();
        auto _val = val;
        while (_val.starts_with("..\\") || _val.starts_with("../")) {
            path = path.parent_path();
            _val = _val.substr(3);
        }
        input_file = path.string() + "/" + _val;
    }
    if (!std::filesystem::exists(input_file)) {
        exit = true;
        std::cerr << "File doesn't exist: " << val << std::endl;
    }
    this->input_file_set = true;
}

std::filesystem::path Args::get_current_path() {
    return std::filesystem::current_path();
}

} // namespace args
