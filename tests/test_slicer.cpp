#include<iostream>
#include "../source/mutavol"

int test_slicer(const int argc, char *argv[]) {
    mtv::Slicer &slicer = mtv::Slicer::get_instance();

    slicer.slice();
    for (const auto &token: slicer.get_tokens()) {
        std::wcout << "token : " << token << std::endl;
    }
    return 0;
}
