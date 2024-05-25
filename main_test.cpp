#include "tests/test_sources.hpp"
#include <iostream>


void separator();

int main(int argc, char *argv[]) {
    std::cout << "Running tests\n";
    auto t_reader{test_reader(argc, argv)};
    std::cout << "Test Reader exit code: " << t_reader << "\n";
    separator();
    //auto t_classifier{test_classifier(argc, argv)};
    //std::cout << "Test Classifier exit code: " << t_classifier << "\n";
    separator();
    return 0;
}

void separator() {
    for (int i{0}; i < 20; i++) {
        std::cout << "-";
    }
    std::cout << "\n";
}
