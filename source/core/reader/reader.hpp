#pragma once
#ifndef READER_HPP
#define READER_HPP

#include <string>
#include <utility>
#include <vector>
#include <ranges>

namespace mtv {
// Using space
using std::string;
using vec_wchar = std::vector<wchar_t>;
// Using space
class Reader {
  public:
    explicit Reader(string input_file) : input_file(std::move(input_file)) {}
    bool read();
    vec_wchar get_buffer() { return buffer; }
  private:
    string input_file;
    vec_wchar buffer;

    bool verify() const;
    bool read_file();
    void clean();
}; // class Reader
} // namespace mtv

#endif // READER_HPP