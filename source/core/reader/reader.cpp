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
        using pair = std::pair<wchar_t, Position>;
        std::wifstream file(input_file);
        if (!file.is_open()) {
            return false;
        }
        auto _file = std::vector(std::istreambuf_iterator(file),
                                 std::istreambuf_iterator<wchar_t>());
        size_t row{1}, column{1};
        auto &buff = Buffer<LinkedList<pair> >::get_instance();
        const auto ll = ListFactory::create_linked_list<pair>();
        for (const auto &c: _file) {
            ll->push(std::make_pair(c, Position(row, column)));
            if (c == L'\n') {
                buff.push(*ll);
                ++row;
                column = 1;
                ll->clear();
            } else {
                ++column;
            }
        }
        return true;
    }

    void Reader::clean() {
        remove_comments();
        remove_lines();
    }

    void Reader::remove_comments() {
        auto &buff = Buffer<LinkedList<std::pair<wchar_t, Position> > >::get_instance();
        size_t line_comment{0};
        bool block_comment{false};
        for (auto &ll: buff) {
            size_t index{0};
            auto it = ll.begin();
            while (it != ll.end()) {
                if (it->first == L'/' && (it + 1) != ll.end() && (it + 1)->first ==
                    L'/') {
                    line_comment = it->second.row;
                }
                if (it->second.row == line_comment && it->first != L'\n') {
                    ++it;
                    ll.pop(index);
                    continue;
                }
                ++index;
                ++it;
            }
        }
        for (auto &ll: buff) {
            size_t index{0};
            auto it = ll.begin();
            while (it != ll.end()) {
                if (it->first == L'/' && (it + 1) != ll.end() && (it + 1)->first ==
                    L'*') {
                    block_comment = true;
                }
                if (block_comment) {
                    if (it->first == L'*' && (it + 1) != ll.end() && (it + 1)->first ==
                        L'/') {
                        block_comment = false;
                        ++it++;
                        ll.pop(index);
                        ll.pop(index);
                    } else {
                        ++it;
                        ll.pop(index);
                    }
                } else {
                    ++index;
                    ++it;
                }
            }
        }
    }

    void Reader::remove_lines() {
        auto &buff = Buffer<LinkedList<std::pair<wchar_t, Position> > >::get_instance();
        size_t index{0};
        auto it{buff.begin()};
        while (it != buff.end()) {
            auto &line = *it;
            auto del{true};
            for (auto &c: line) {
                if (c.first != L' ' && c.first != L'\n' && c.first != L'\t') {
                    del = false;
                    break;
                }
            }
            if (del) {
                ++it;
                buff.pop(index);
                continue;
            }
            ++index;
            ++it;
        }
    }
} // namespace mtv
