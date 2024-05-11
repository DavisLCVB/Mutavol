#include "../source/mutavol"
#include <iostream>

int main(int argc, char *argv[]) {
    auto args = mtv::Args(argc, argv);
    args.recon();
    if (args.should_exit()) {
        return 0;
    }
    auto output_file = args.get_output_file();
    auto input_file = args.get_input_file();
    auto reader = mtv::Reader(input_file);
    if (!reader.read()) {
        return 1;
    }
    auto buffer = reader.get_buffer();
    for (auto &c : buffer) {
        std::wcout << c;
    }
    return 0;
}