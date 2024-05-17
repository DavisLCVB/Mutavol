#include "reader.hpp"
#include <iostream>
#include <fstream>
#include <ranges>

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

    bool Reader::verify() const {
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
        this->buffer = vec_wchar(std::istreambuf_iterator(file),
                                 std::istreambuf_iterator<wchar_t>());
        file.close();
        return true;
    }

    void Reader::clean() {
        auto new_buff = vec_wchar();
        auto it = this->buffer.begin();
        while (it != this->buffer.end()) {
            if (*it == '/' && *(it + 1) == '/') {
                while (*it != '\n') {
                    ++it;
                }
            } else if (*it == '/' && *(it + 1) == '*') {
                while (!(*it == '*' && *(it + 1) == '/')) {
                    ++it;
                }
                it += 2;
            } else {
                new_buff.push_back(*it);
                ++it;
            }
        }

        std::vector<std::pair<size_t, size_t> > to_remove;
        do {
            to_remove.clear();
            for (size_t j = 0; j < new_buff.size(); ++j) {
                if (new_buff[j] == L'\n') {
                    std::pair<size_t, size_t> p;
                    p.first = j++;
                    while (j < new_buff.size() && new_buff[j] == L' ') {
                        ++j;
                    };
                    if (new_buff[j] == L'\n' || new_buff[j] == L' ') {
                        p.first == j - 1 ? p.second = j : p.second = j - 1;
                        to_remove.push_back(p);
                    }
                }
            }
            for (auto &[fst, snd]: std::ranges::reverse_view(to_remove)) {
                new_buff.erase(new_buff.begin() + fst, new_buff.begin() + snd);
            }
        } while (!to_remove.empty());
        this->buffer = new_buff;
    }
} // namespace mtv
