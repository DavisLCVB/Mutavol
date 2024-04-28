#include "../source/args/args.hpp"
#include "../source/files/files.hpp"
#include <iostream>
#include <locale.h>

int main(int argc, char *argv[]) {
    auto args = mtv::Args(argc, argv);
    args.recon();
    if (args.should_exit()) {
        return 0;
    }
    auto output_file = args.get_output_file();
    std::cout << "Output file: " << output_file << std::endl;
    auto input_file = args.get_input_file();
    std::cout << "Input file: " << input_file << std::endl;
    mtv::File file(input_file);
    file.read();
    auto tokens = file.get_tokens();
    for (auto token : tokens) {
        std::wcout << token;
    }
    return 0;
}