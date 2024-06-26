#include "../source/mutavol"
#include "../source/utils/format/format.hpp"
#include <iostream>


int coin_test(const int argc, char *argv[]) {
    const auto result = mtv::Args::process_args(argc, argv);
    if (!result.args_processed) return -1;
    if (result.should_exit) return 0;
    mtv::Scanner::init_scanner(result.input);
    while(true) {
        const auto token = mtv::Scanner::get();
        std::wcout << token <<L'\n';
        if(token.lexem == L"$")break;
    }
    return 0;
}

int test_parser(const int argc, char *argv[]) {
    const auto result = mtv::Args::process_args(argc, argv);
    if (!result.args_processed) return -1;
    if (result.should_exit) return 0;
    mtv::Scanner::init_scanner(result.input);

    auto &parser = mtv::Parser::get_instance();
    parser.parse();

    return 0;
}

int test_interpreter() {

    auto &parser = mtv::Iparser::get_instance();
    parser.parse();

    return 0;
}