#include "scanner.hpp"

namespace scanner {
    std::unique_ptr<Scanner> Scanner::instance = nullptr;
    std::once_flag Scanner::init_instance_flag;

    Scanner &Scanner::get_instance() {
        std::call_once(init_instance_flag, &Scanner::init_instance, "");
        return *instance;
    }

    Scanner &Scanner::get_instance(std::string &source_string) {
        std::call_once(init_instance_flag, &Scanner::init_instance, source_string);
        return *instance;
    }

    void Scanner::init_instance(std::string &source_string) {
        instance.reset(new Scanner(source_string));
    }

    Scanner::Scanner(std::string &source_string) : source_string(source_string) {
        current = source_string.begin();
        end = source_string.end();
    }

    void Scanner::scan() {
        
    }
    const std::string Scanner::OPERATORS = "+-*/%";

}