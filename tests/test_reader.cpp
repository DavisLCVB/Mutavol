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
    if (const auto &reader = mtv::Reader::get_instance(r.input); !reader.read()) return 1;
    for (auto &buff = mtv::Buffer<mtv::LinkedList<std::pair<wchar_t,
             mtv::Position> > >::get_instance(); auto &line: buff) {
        for (const auto &[fst, snd]: line) {
            std::wcout << fst;
        }
    }
    std::cout << "\n";
    return 0;
}
