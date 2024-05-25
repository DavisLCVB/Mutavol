#include "../source/core/slicer/slicer.hpp"
#include<iostream>
#include<cassert>
#include "../source/core/buffer/buffer.hpp"
#include "../source/utilities/data_structures/position.hpp"
int test_slicer(const int argc, char *argv[]){

    mtv::slicer &slicer = mtv::slicer::get_instance();

    slicer.slice();
    for(const auto &token : slicer.get_tokens()){
        std::wcout <<"token : "<< token << std::endl;
    }
    return 0;
}
