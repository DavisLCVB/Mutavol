#include "../source/mutavol"
#include <iostream>

int test_reader(const int argc, char *argv[]) {
    const auto r = mtv::Args::process_args(argc, argv);
    if(!r.args_processed) {
        return 1;
    }
    if (r.should_exit) {
        return 0;
    }
    return 0;
}
