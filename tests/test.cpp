#include "../source/mutavol"
#include <iostream>

#include "../source/core/buffer/buffer.hpp"
#include "../source/utilities/data_structures/position.hpp"

int main(const int argc, char *argv[]) {
    auto &args = mtv::Args::get_instance(argc, argv);
    args.recon();
    if (args.should_exit()) {
        return 0;
    }
    auto output_file = args.get_output_file();
    const auto input_file = args.get_input_file();
    if (const auto &reader = mtv::Reader::get_instance(input_file); !reader.read()) {
        return 1;
    }
    auto &buff = mtv::Buffer<std::pair<wchar_t, mtv::Position> >::get_instance();
    const auto size = buff.get_size();
    for (size_t i = 0; i < size; ++i) {
        auto [c, p] = buff.get(i);
        std::wcout << c << " " << p.row << " " << p.column << std::endl;
    }
    return 0;
}
