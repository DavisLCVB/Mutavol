#include "reader.hpp"

namespace mtv {

bool Reader::read() {
    if (!verify()) {
        std::cerr << "Error: Invalid input file." << std::endl;
        return false;
    }
    if (!read_file()) {
        std::cerr << "Error: Could not read input file." << std::endl;
        return false;
    }
    try {
        clean();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool Reader::verify() {
    std::ifstream file(input_file);
    if (!file.is_open()) {
        return false;
    }
    file.close();
    return true;
}

bool Reader::read_file() {
    std::wifstream file(input_file);
    if (!file.is_open()) {
        return false;
    }
        this->buffer = vec_wchar(std::istreambuf_iterator<wchar_t>(file),
                                 std::istreambuf_iterator<wchar_t>());
    file.close();
    return true;
}

void Reader::clean(){
    auto new_buff = vec_wchar();
    auto it = this->buffer.begin();
    while (it != this->buffer.end()) {
        if(*it == '/' && *(it + 1) == '/') {
            while (*it != '\n') {
                ++it;
            }
        } else if(*it == '/' && *(it + 1) == '*') {
            while (!(*it == '*' && *(it + 1) == '/')) {
                ++it;
            }
            it += 2;
        } else {
            new_buff.push_back(*it);
            ++it;
        }
    }
    this->buffer = new_buff;
}

} // namespace mtv