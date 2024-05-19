#include "reader.hpp"
#include <iostream>
#include <fstream>
#include <ranges>
#include <vector>

#include "../../utilities/data_structures/position.hpp"
#include "../buffer/buffer.hpp"

namespace mtv {

    std::unique_ptr<Reader> Reader::instance = nullptr;

    void Reader::init_instance(const std::string &input_file) {
        instance.reset(new Reader(input_file));
    }

    Reader &Reader::get_instance(const std::string &input_file) {
        if (instance == nullptr) {
            init_instance(input_file);
        }
        return *instance;
    }

    bool Reader::read() const {
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

    bool Reader::read_file() const {
        std::wifstream file(input_file);
        if (!file.is_open()) {
            return false;
        }
        auto _file = std::vector(std::istreambuf_iterator(file),
                                 std::istreambuf_iterator<wchar_t>());
        size_t i{1};
        size_t j{1};
        for (auto wc: _file) {
            Buffer<std::pair<wchar_t, Position> >::get_instance().push(
                std::make_pair(wc, Position(i, j)));
            if (wc == L'\n') {
                ++i;
                j = 1;
            } else {
                ++j;
            }
        }
        file.close();
        return true;
    }

    void Reader::clean() {
        remove_comments();
        remove_spaces();
    }

    void Reader::remove_comments() {
        auto &buff = Buffer<std::pair<wchar_t, Position> >::get_instance();
        auto it = buff.begin();
        bool in_block{false};
        unsigned long long line_n{0};
        while(it != buff.end()) {
            std::wstring line;
            while (it != buff.end() && it->second.row == line_n) {
                line.push_back(it->first);
                ++it;
            }
            size_t pos_line_comment = line.find(L"//");
            if (pos_line_comment != std::wstring::npos) {
                auto init_comment = buff.begin() +
            }
        }
    }

    void Reader::remove_spaces() {
    }
} // namespace mtv
