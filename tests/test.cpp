#include "../source/mutavol"
#include <iostream>

int main(const int argc, char *argv[]) {
    mtv::Args &args = mtv::Args::get_instance(argc, argv);
    args.recon();
    if (args.should_exit()) {
        return 0;
    }
    auto output_file = args.get_output_file();
    const auto input_file = args.get_input_file();
    auto reader = mtv::Reader(input_file);
    if (!reader.read()) {
        return 1;
    }
    for (auto buffer = reader.get_buffer(); const auto &c: buffer) {
        std::wcout << c;
    }
    return 0;
}
