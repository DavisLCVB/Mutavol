#include "source/mutavol"

int main(int argc, char *argv[]) {
    if (argc > 1) {
        const auto result = mtv::Args::process_args(argc, argv);
        if (!result.args_processed) return -1;
        if (result.should_exit) return 0;
    }
    auto &parser = mtv::Iparser::get_instance();
    parser.parse();
    return 0;
}
