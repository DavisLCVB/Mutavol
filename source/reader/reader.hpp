#pragma once
#ifndef READER_HPP
#define READER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <ranges>

namespace mtv {
// Using space
using std::string;
using vec_wchar = std::vector<wchar_t>;
// Using space
class Reader {
  public:
    Reader(string input_file) : input_file(input_file) {}
    bool read();
    vec_wchar get_buffer() { return buffer; }
  private:
    string input_file;
    vec_wchar buffer;

    bool verify();
    bool read_file();
    void clean();
}; // class Reader
} // namespace mtv

#endif // READER_HPP