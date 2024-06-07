#include "tests/test_sources.hpp"
#include <iostream>


void separator();

int main(const int argc, char *argv[]) {
    coin_test(argc, argv);
    return 0;
}

void separator() {
    for (int i{0}; i < 20; i++) {
        std::cout << "-";
    }
    std::cout << "\n";
}