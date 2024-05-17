#include "args.hpp"

namespace mtv {

    std::once_flag Args::init_instance_flag;
    std::unique_ptr<Args> Args::instance = nullptr;

    Args &Args::get_instance() {
        std::call_once(init_instance_flag, &Args::init_instance, 0, nullptr);
        return *instance;
    }

    Args &Args::get_instance(const int argc, char *argv[]) {
        std::call_once(init_instance_flag, &Args::init_instance, argc, argv);
        return *instance;
    }

    Args::Args(const int argc, char *argv[]) {
        for (int i = 1; i < argc; i++) {
            args.emplace_back(argv[i]);
        }
        exit = false;
    }

    void Args::init_instance(const int argc, char *argv[]) {
        instance.reset(new Args(argc, argv));
    }


    void Args::recon() {
        if (args.empty()) {
            exit = true;
            return;
        }
        auto arg = get();
        while (!arg.empty()) {
            if (templates.contains(arg)) {
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
        std::string sep = "/";
#ifdef _WIN32
        sep = "\\";
#endif // _WIN32
        if (output_file.empty()) {
            if (output_file.empty()) {
                output_file = get_current_path().string() + sep + val;
            } else {
                exit = true;
                std::cerr << "Invalid argument: " << val << std::endl;
            }
            if (val.empty()) {
                exit = true;
                std::cerr << "Invalid argument: " << val << std::endl;
            }
            if (val.starts_with(".\\") || val.starts_with("./")) {
                output_file = val.substr(2);
                output_file = get_current_path().string() + sep + output_file;
            }
#ifdef __linux__
        if (val.starts_with("\\") || val.starts_with("/")) {
            output_file = val;
        }
#endif// __linux__
#ifdef _WIN32
            if (const std::string vol = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
                val.find(':') != std::string::npos &&
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
                output_file = path.string() + sep + _val;
            }
            for (auto &c: output_file) {
                if (c == '\\' || c == '/') {
                    c = sep[0];
                }
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
        std::string sep = "/";
#ifdef _WIN32
        sep = "\\";
#endif// _WIN32
        if (input_file.empty()) {
            input_file = get_current_path().string() + sep + val;
        } else {
            exit = true;
            std::cerr << "Invalid argument: " << val << std::endl;
        }
        if (val.empty()) {
            exit = true;
            std::cerr << "Invalid argument: " << val << std::endl;
        }
        if (val.starts_with(".\\") || val.starts_with("./")) {
            input_file = val.substr(2);
            input_file = get_current_path().string() + sep + input_file;
        }
#ifdef __linux__
    if (val.starts_with("\\") || val.starts_with("/")) {
        input_file = val;
    }
#endif// __linux__
#ifdef _WIN32
        if (const std::string vol = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            val.find(':') != std::string::npos &&
            vol.find(val[0]) != std::string::npos) {
            input_file = val;
        }
#endif// _WIN32
        if (val.starts_with("..\\") || val.starts_with("../")) {
            auto path = get_current_path();
            auto _val = val;
            while (_val.starts_with("..\\") || _val.starts_with("../")) {
                path = path.parent_path();
                _val = _val.substr(3);
            }
            input_file = path.string() + sep + _val;
        }
        this->input_file_set = true;
        for (auto &c: input_file) {
            if (c == '\\' || c == '/') {
                c = sep[0];
            }
        }
        if (!std::filesystem::exists(input_file)) {
            exit = true;
            std::cerr << "File doesn't exist: " << val << std::endl;
        }
    }

    std::filesystem::path Args::get_current_path() {
        return std::filesystem::current_path();
    }
} // namespace mtv
