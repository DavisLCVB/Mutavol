#include <iostream>
#include "config.hpp"

int main(int argc, char* argv[]){
    std::cout << config::project << std::endl;
    std::cout << "Version: " << config::version << std::endl;
    return 0;
}