#include "source/mutavol"

int main(int argc, char* argv[]){
    const auto result = mtv::Args::process_args(argc, argv);
    if (!result.args_processed) return -1;
    if (result.should_exit) return 0;
    mtv::Scanner::init_scanner(result.input);
    auto &parser = mtv::Parser::get_instance();
    parser.parse();
    return 0;
}