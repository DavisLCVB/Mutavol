#pragma once
#ifndef FILES_HPP
#define FILES_HPP

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace mtv {
class File {
  public:
    File() = default;
    File(const std::string &file);
    void set_file(const std::string &file);
    void write(const std::string &data);
    void read();
    std::vector<wchar_t> get_tokens() const;

  private:
    std::string file;
    std::vector<wchar_t> tokens;
};
} // namespace mtv

#endif // FILES_HPP