#include "reader.hpp"
#include <iostream>
#include <fstream>
#include <ranges>
#include <vector>

#include "../../../utils/utils.hpp"
#include "../../buffer/buffer.hpp"

namespace mtv {
    std::unique_ptr<Scanner::Reader> Scanner::Reader::instance = nullptr;

    void Scanner::Reader::init_instance(const std::string &input_file) {
        instance.reset(new Reader(input_file));
    }

    //PARA EL INTERPRETE
    void Scanner::Reader::init_instance() {
        instance.reset(new Reader());
    }

    Scanner::Reader &Scanner::Reader::get_instance(const std::string &input_file) {
        if (instance == nullptr) {
            init_instance(input_file);
        }
        return *instance;
    }

    //PARA EL INTERPRETE
    Scanner::Reader &Scanner::Reader::get_instance() {
        if (instance == nullptr) {
            init_instance();
        }
        return *instance;
    }

    bool Scanner::Reader::read() const {
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

    bool Scanner::Reader::verify() const {
        std::ifstream file(input_file);
        if (!file.is_open()) {
            return false;
        }
        file.close();
        return true;
    }

    bool Scanner::Reader::read_file() const {
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
        if (!ll->is_empty()) {
            buff.push(*ll);
        }
        delete ll;
        return true;
    }

    bool Scanner::Reader::read_cin(const std::wstring &line) {
        using pair = std::pair<wchar_t, Position>;
        size_t row{1}, column{1};
        auto &buff = Buffer<LinkedList<pair> >::get_instance();
        const auto ll = ListFactory::create_linked_list<pair>();
        for (const auto &c: line) {
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
        if (!ll->is_empty()) {
            buff.push(*ll);
        }
        delete ll;
        return true;
    }

    void Scanner::Reader::clean() {
        remove_comments();
        remove_lines();
    }

    void Scanner::Reader::remove_comments() {
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
                        ++it;
                        ++it;
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

    void Scanner::Reader::remove_lines() {
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
