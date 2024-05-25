#include "../source/mutavol"
#include <iostream>

int test_reader(const int argc, char *argv[]) {
    const auto r = mtv::Args::process_args(argc, argv);
    if (!r.args_processed) {
        return 1;
    }
    if (r.should_exit) {
        return 0;
    }
    auto &reader = mtv::Reader::get_instance(r.input);
    if (!reader.read()) return 1;
    auto &buff = mtv::Buffer<mtv::LinkedList<std::pair<wchar_t,
        mtv::Position> > >::get_instance();
    for(auto &line : buff) {
        for(auto &pair : line) {
            std::wcout << pair.first;
        }
    }
    std::cout << "\n";
    return 0;
}
