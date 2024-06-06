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
        if(token.lexem.empty())break;
        std::wcout << token <<L'\n';
    }
    return 0;
}