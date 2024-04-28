#include <iostream>
#include "../source/args/args.hpp"

int main(int argc, char *argv[]){
    auto args = args::Args(argc, argv);
    args.recon();
    if(args.should_exit()){
        return 0;
    }
    auto output_file = args.get_output_file();
    std::cout << "Output file: " << output_file << std::endl;
    auto input_file = args.get_input_file();
    std::cout << "Input file: " << input_file << std::endl;
}