#include "../source/args/args.hpp"
#include "../source/files/files.hpp"
#include "../source/scanner/scanner.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    auto args = mtv::Args(argc, argv);
    args.recon();
    if (args.should_exit()) {
        return 0;
    }
    auto output_file = args.get_output_file();
    std::cout << "Output file: " << output_file << "\n";
    auto input_file = args.get_input_file();
    std::cout << "Input file: " << input_file << "\n";
    mtv::File file(input_file);
    file.read();
    auto tokens = file.get_tokens();
    for (auto token : tokens) {
        std::wcout << token;
    }
    std::wcout << std::endl;
    mtv::Scanner scanner(tokens);
    scanner.scan();
    auto scanned_tokens = scanner.get_pre_tokens();
    for (auto token : scanned_tokens) {
        std::wcout << token << std::endl;
    }
}